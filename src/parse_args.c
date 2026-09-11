#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include "const.h"
#include "parse_args.h"

opts_t	*parse_init_opts()
{
	static opts_t	opts[256];

	return opts;
}

void	parse_set_accepted(const char *s, opts_t *opts)
{
	for (size_t i = 0; i < strlen(s); ++i)
	{
		opts[(size_t)s[i]].accepted = true;
		opts[(size_t)s[i]].pos = 0;
		opts[(size_t)s[i]].dest = NULL;
		opts[(size_t)s[i]].parse = NULL;
	}
}

void	parse_set_parsing(char c, void *dest, int (*f)(const char *, void *), opts_t *opts)
{
	opts[(size_t)c].dest = dest;
	opts[(size_t)c].parse = f;
}

ssize_t	parse_args(size_t argc, char **argv, opts_t *opts)
{
	size_t		opt_idx = 1;
	size_t		write_idx = 0;
	bool		is_arg = false;
	unsigned	c;

	for (size_t i = 1; i < argc; ++i)
	{
		const char	*cur = argv[i];
		
		if (is_arg)
		{
			if (opts[c].parse(argv[i], opts[c].dest))
				return -1;
			is_arg = false;
		}
		else if (cur[0] != '-' || (cur[0] == '-' && cur[1] == '\0'))
			argv[write_idx++] = argv[i];
		else if (cur[1] == '-' && cur[2] == '\0')
		{
			memcpy(argv + write_idx, argv + i + 1, (argc - i) * sizeof(char *));
			write_idx += (argc - i) - 1; 
			break;
		}
		else
		{
			for (size_t j = 1; j < strlen(cur); ++j)
			{
				c = cur[j];
				if (!opts[c].accepted)
				{
					dprintf(2, "%s: invalid option -- '%c'\n", PROG_NAME, c);
					return -1;
				}
				opts[c].pos = ++opt_idx;
				if (opts[c].parse == NULL)
					continue;
				if (cur[j + 1] != '\0')
				{
					if (opts[c].parse(argv[i] + j + 1, opts[c].dest))
						return -1;
					break;
				}
				else
					is_arg = true;
			}
		}
	}
	argv[write_idx] = NULL;
	if (is_arg)
	{
		dprintf(2, "%s: option requires an argument -- '%c'\n", PROG_NAME, c);
		return -1;
	}
	return write_idx;
}
