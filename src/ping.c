#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "const.h"
#include "ping.h"

unsigned short	checksum(void *b, size_t len)
{
    unsigned short	*buf = b;
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
	printf("chksm %hu\n", ~sum);
    return ~sum;
}

int	do_ping(char **argv, specs_t *specs, int sock)
{
	ping_t	pkt;
	char	buf[256];
	int		ret = 0;
	
	errno = 0;
	memset(&pkt, 0, sizeof(pkt));
	pkt.hdr.type = ICMP_ECHO;
	pkt.hdr.un.echo.id = getpid(); //??
	for (size_t i = 0; i < sizeof(pkt.msg); ++i)
		pkt.msg[i] = '0' + i;
	pkt.msg[sizeof(pkt.msg) - 1] = 0;
	pkt.hdr.un.echo.sequence = 0;
	pkt.hdr.checksum = checksum(&pkt, sizeof(pkt));
	checksum(&pkt, sizeof(pkt));



	struct sockaddr addr = {AF_INET, {htons(0), inet_addr("1.1.1.1")}};
	struct sockaddr_in r_addr;
	unsigned					addr_len = 0;

	memset(&r_addr, 0, sizeof(r_addr));
	ret = sendto(sock, &pkt, sizeof(pkt), 0, &addr, 16);
	printf("ret is %d, %s\n", ret, strerror(errno));
	ret = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&r_addr, &addr_len);
	printf("ret is %d, %s\n", ret, strerror(errno));
	printf("'''%s'''\n", buf + sizeof(pkt.hdr));
	printf("'''checksum %hu'''\n", ((ping_t *)&buf)->hdr.checksum);
	((ping_t *)&buf)->hdr.checksum = 0;
	printf("'''checksum %hu'''\n", checksum(buf, 64));

//ping dans l'ordre
//si host inconnu on arrete tout
//si ctrl c on arrete tout
	//f((src_hp = gethostbyname(src_name)) == NULL)
	//ret = sendto(sock, send_buf, 64, 0, (struct sockaddr){sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("1.1.1.1")}, 16);
	return 0
}
