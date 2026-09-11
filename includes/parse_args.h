#ifndef PARSE_ARGS_H
# define PARSE_ARGS_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

typedef	struct opt_s
{
	int		(*parse)(const char *, void *);
	void	*dest;
	size_t	pos;
	bool	accepted;
}	opts_t;

ssize_t	parse_args(size_t argc, char **argv, opts_t *opts);
void	parse_set_accepted(const char *s, opts_t *opts);
void	parse_set_parsing(char c, void *dest, int (*f)(const char *, void *), opts_t *opts);
opts_t	*parse_init_opts();

#endif
