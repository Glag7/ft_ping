#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "parse_args.h"
#include "parsing_funcs.h"
#include "specs.h"
#include "const.h"
#include "ping.h"

volatile sig_atomic_t	g_flag = 0;

static ssize_t	get_specs(int argc, char **argv, opts_t *opts, specs_t *specs)
{
	ssize_t n = 0;
	
	specs->verbose = false;
	specs->count = -1ULL;
	specs->interval = 1;
	specs->timeout = -1ULL;
	specs->linger = 1;
	
	parse_set_accepted("?vqicwW", opts);
	parse_set_parsing('c', &specs->count, wr_strtol, opts);
	parse_set_parsing('i', &specs->interval, wr_strtol, opts);
	parse_set_parsing('w', &specs->timeout, wr_strtol_max, opts);
	parse_set_parsing('W', &specs->linger, wr_strtol_max, opts);
	n = parse_args(argc, argv, opts);
	specs->verbose = opts['v'].pos;
	specs->quiet = opts['v'].pos;
	return n;
}

void	set_flag(int sig)
{
	(void)sig;
	g_flag = sig;
}

static int	set_sigaction()
{
	struct sigaction	sig;

	memset(&sig, 0, sizeof(sig));
	sig.sa_handler = set_flag;
	return sigaction(SIGINT, &sig, NULL);
}

int	main(int argc, char **argv)
{
	specs_t	specs;
	opts_t	*opts = parse_init_opts();
	ssize_t n = get_specs(argc, argv, opts, &specs);
	int 	sock;
	int		ret = 1;
	
	g_flag = 0;
	if (n == -1)
		return 1;
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock == -1)
	{
		dprintf(2, "%s: cannot open socket: %s\n", PROG_NAME, strerror(errno));
		return 2;
	}
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &ret, 4);
	if (set_sigaction())
	{
		dprintf(2, "%s: cannot set signal handler: %s\n", PROG_NAME, strerror(errno));
		close(sock);
		return 2;
	}
	ret = do_ping(argv, &specs, sock);
	close(sock);
	return ret;
}
