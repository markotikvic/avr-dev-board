/*
 * util.c
 *
 * Created: 7/1/2015 6:12:44 PM
 *  Author: Lazar
 */ 

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
