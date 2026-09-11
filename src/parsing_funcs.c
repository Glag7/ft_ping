#include <stdlib.h>
#include <stdio.h>
#include "const.h"

int	wr_strtol(const char *s, void *dest)
{
	char	*end;
	
	*((size_t *) dest) = strtol(s, &end, 10);
	if (*end != '\0')
		dprintf(2, "%s: invalid value (`%s' near `%s')\n", PROG_NAME, s, end);
	return *end != '\0';
}

int	wr_strtol_max(const char *s, void *dest)
{
	char	*end;
	size_t	n;
	
	n = strtol(s, &end, 10);
	*((size_t *) dest) = n;
	if (*end != '\0')
		dprintf(2, "%s: invalid value (`%s' near `%s')\n", PROG_NAME, s, end);
	else if (n > 2147483647)
		dprintf(2, "%s: option value too big: %s\n", PROG_NAME, s);
	return *end != '\0' || n > 2147483647;
}
