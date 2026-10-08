#include <stdio.h>
#include <stdlib.h>
#include "parse_args.h"
#include "parsing_funcs.h"
#include "specs.h"

ssize_t	get_specs(int argc, char **argv, opts_t *opts, specs_t *specs)
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
	return n;
}

#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include "const.h"

typedef struct s_ping
{
	struct icmphdr	hdr;
	char			msg[64 - sizeof(struct icmphdr)];
}	t_ping;

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int	main(int argc, char **argv)
{
	specs_t	specs;
	opts_t	*opts = parse_init_opts();
	ssize_t n = get_specs(argc, argv, opts, &specs);
	
	if (n == -1)
		return 1;

	
	printf("%zd args\n", n);
	for (ssize_t i = 0; i < n; ++i)
	{
		printf("'%s'\n", argv[i]);
	}
	printf("opts\n");
	printf("verbose: %d\n", specs.verbose);
	printf("count: %zu\n", specs.count);
	printf("interval: %zu\n", specs.interval);
	printf("timeout: %zu\n", specs.timeout);
	printf("linger: %zu\n", specs.linger);


	int 	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int		ret = 1;

	if (sock == -1)
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock == -1)
	{
		dprintf(2, "%s: cannot open socket: %s\n", PROG_NAME, strerror(errno));
		return 2;
	}
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &ret, 4);

	t_ping	pkt;
	char	buf[256];
	
	memset(&pkt, 0, sizeof(pkt));
	pkt.hdr.type = ICMP_ECHO;
	pkt.hdr.un.echo.id = getpid(); //??
	for (size_t i = 0; i < sizeof(pkt.msg); ++i)
		pkt.msg[i] = '0' + i;
	pkt.msg[sizeof(pkt.msg) - 1] = 0;
	pkt.hdr.un.echo.sequence = 0;
	pkt.hdr.checksum = checksum(&pkt, sizeof(pkt));


	struct sockaddr addr = {AF_INET, {htons(0), inet_addr("1.1.1.1")}};
	struct sockaddr_in r_addr;
	unsigned					addr_len = 0;

	memset(&r_addr, 0, sizeof(r_addr));
	ret = sendto(sock, &pkt, sizeof(pkt), 0, &addr, 16);
	printf("ret is %d, %s\n", ret, strerror(errno));
	ret = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&r_addr, &addr_len);
	printf("ret is %d, %s\n", ret, strerror(errno));
	printf("'''%s'''\n", buf + 32);

	//construire la structure ip de merde

	//f((src_hp = gethostbyname(src_name)) == NULL)
	//ret = sendto(sock, send_buf, 64, 0, (struct sockaddr){sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("1.1.1.1")}, 16);



	close(sock);
	//rt_sigagtion
	//sendto
	//pselect
	//recvfrom
}

//struct ip
//  {
//#if __BYTE_ORDER == __LITTLE_ENDIAN
//    unsigned int ip_hl:4;                /* header length */
//    unsigned int ip_v:4;                /* version */
//#endif
//#if __BYTE_ORDER == __BIG_ENDIAN
//    unsigned int ip_v:4;                /* version */
//    unsigned int ip_hl:4;                /* header length */
//#endif
//    u_int8_t ip_tos;                        /* type of service */
//    u_short ip_len;                        /* total length */
//    u_short ip_id;                        /* identification */
//    u_short ip_off;                        /* fragment offset field */
//#define        IP_RF 0x8000                        /* reserved fragment flag */
//#define        IP_DF 0x4000                        /* dont fragment flag */
//#define        IP_MF 0x2000                        /* more fragments flag */
//#define        IP_OFFMASK 0x1fff                /* mask for fragmenting bits */
//    u_int8_t ip_ttl;                        /* time to live */
//    u_int8_t ip_p;                        /* protocol */
//    u_short ip_sum;                        /* checksum */
//    struct in_addr ip_src, ip_dst;        /* source and dest address */
//  };
