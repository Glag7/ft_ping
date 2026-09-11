#include "parse_args.h"
#include "parsing_funcs.h"
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	opts_t	*opts = parse_init_opts();
	ssize_t n = 0;
	
	size_t	foo = 0;
	parse_set_accepted("?vqicwW", opts);
	parse_set_parsing('c', &foo, wr_strtol, opts);
	parse_set_parsing('i', &foo, wr_strtol, opts);
	parse_set_parsing('w', &foo, wr_strtol_max, opts);
	parse_set_parsing('W', &foo, wr_strtol_max, opts);
	n = parse_args(argc, argv, opts);

	if (n == -1)
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
