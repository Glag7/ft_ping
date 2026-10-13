#ifndef SPECS_H
# define SPECS_H

#include <stddef.h>
#include <stdbool.h>

typedef struct specs_s
{
	size_t	count;
	size_t	interval;
	size_t	timeout;
	bool	verbose;
	bool	quiet;
}	specs_t;

#endif 
