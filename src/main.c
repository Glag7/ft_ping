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
	
}
