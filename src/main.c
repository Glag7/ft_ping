#include "parse_args.h"
#include <stdio.h>
#include <stdlib.h>

int	foo(const char *s, void *dest)
{
	//check null ?
	*((size_t *) dest) = strtol(s, NULL, 10);//todo error checking
	return 0;
}

int	main(int argc, char **argv)
{
	opts_t	*opts = parse_init_opts();
	ssize_t n = 0;
	
	parse_set_accepted("?vqicwW", opts);
	//set les parsings icwW
	n = parse_args(argc, argv, opts);

	if ( n == -1)
		return 1;
	printf("%zd args\n", n);
	for (ssize_t i = 0; i < n; ++i)
	{
		printf("'%s'\n", argv[i]);
	}
	printf("opts\n");
	for (ssize_t i = 0; i < 256; ++i)
	{
		if (opts[i].pos > 0)
			printf("'%c'\n", i);
	}
	
}
