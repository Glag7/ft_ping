#ifndef PING_H
# define PING_H

#include <netinet/ip_icmp.h>
#include "specs.h"

typedef struct ping_s
{
	struct icmphdr	hdr;
	char			msg[64 - sizeof(struct icmphdr)];
}	ping_t;

int	do_ping(char **argv, specs_t *specs, int sock);

#endif
