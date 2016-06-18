#include <assert.h>
#include <stdio.h>
#include "util.h"

void ulongtoa(char *dest, unsigned long val)
{
	const int n = snprintf(0, 0, "%lu", val);
	assert(n > 0);
	int c = snprintf(dest, n+1, "%lu", val);
	assert(dest[n] == '\0');
	assert(c == n);	
}
