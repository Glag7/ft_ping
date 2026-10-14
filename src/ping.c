#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>
#include "const.h"
#include "ping.h"

extern volatile sig_atomic_t	g_flag;

static int	get_addr(struct sockaddr *addr, char *host)
{
	in_addr_t		addr_ip;
	struct addrinfo	*res;
	int				err;
	
	memset(addr, 0, sizeof(*addr));
	addr_ip = inet_addr(host);
	if (addr_ip != -1U)
	{
		*addr = (struct sockaddr){AF_INET, {htons(0), addr_ip}};
		return 0;
	}
	err = getaddrinfo(host, NULL, NULL, &res);
	if (err)
	{
		dprintf(2, "%s: unknown host\n", PROG_NAME);
		return 1;
	}
	memcpy(addr, res->ai_addr, sizeof(*addr));
	freeaddrinfo(res);
	return 0;
}

static unsigned short	checksum(const void *b, size_t len)
{
    const unsigned short	*buf = b;
    unsigned int	sum = 0;

	while (len > 1)
	{
        sum += *buf++;
		len -= 2;
	}
    if (len)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

static void	build_pkt(ping_t *pkt, unsigned short count)
{
	memset(pkt, 0, sizeof(pkt));
	pkt->hdr.type = ICMP_ECHO;
	pkt->hdr.un.echo.sequence = count;
	pkt->hdr.un.echo.id = getpid();
	for (size_t i = 0; i < sizeof(pkt->msg); ++i)//mettre le temps d'envoi dedans ?
		pkt->msg[i] = '0' + i;
	pkt->msg[sizeof(pkt->msg) - 1] = 0;
	pkt->hdr.checksum = checksum(&pkt, sizeof(pkt));
}

static bool	is_delta_bigger(struct timespec *start, struct timespec *end, size_t seconds)
{
	return (size_t)((end->tv_sec - start->tv_sec) * 1000000000 + (end->tv_nsec - start->tv_nsec))
																		> seconds * 1000000000;
}

int	do_ping(char **argv, specs_t *specs, int sock)
{
	struct sockaddr addr;
	ping_t			pkt;
	struct			timespec start, last, now;
	size_t			i = 0;
	
	while (*argv)
	{	
		if (get_addr(&addr, *argv))
			return 1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		clock_gettime(CLOCK_MONOTONIC_RAW, &now);
		i = 0;
		while (!g_flag && i < specs->count && !is_delta_bigger(&start, &now, specs->timeout))
		{
			if (i == 0 || is_delta_bigger(&last, &now, specs->interval))
			{
				build_pkt(&pkt, i);
				++i;
				printf("ping %s\n", *argv);
				clock_gettime(CLOCK_MONOTONIC_RAW, &last);
			}
			//check reception
			clock_gettime(CLOCK_MONOTONIC_RAW, &now);
		}
		printf("print stats %s\n", *argv);
		++argv;
	}
	return 0;
}
//	struct sockaddr addr = {AF_INET, {htons(0), inet_addr("1.1.1.1")}};
//	struct sockaddr_in r_addr;
//	unsigned					addr_len = 0;
//
//	memset(&r_addr, 0, sizeof(r_addr));
//	ret = sendto(sock, &pkt, sizeof(pkt), 0, &addr, 16);
//	printf("ret is %d, %s\n", ret, strerror(errno));
//	ret = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&r_addr, &addr_len);
//	printf("ret is %d, %s\n", ret, strerror(errno));
//	printf("'''%s'''\n", buf + sizeof(pkt.hdr));
//	printf("'''checksum %hu'''\n", ((ping_t *)&buf)->hdr.checksum);
//	((ping_t *)&buf)->hdr.checksum = 0;
//	printf("'''checksum %hu'''\n", checksum(buf, 64));


//for count
//faire le ping
//si temps depasse break
//attendre i secondes ou flood
//q/f et v
//imprimer stats
