/*
 * util.h
 *
 * Created: 6/17/2015 12:17:55 AM
 *  Author: Lazar
 */ 


#ifndef UTIL_H_
#define UTIL_H_


void ulongtoa(char *dest, unsigned long val);

#define SBIT(w, n)	w |= (1 << n)
#define CBIT(w, n)	w &= ~(1 << n)
#define TBIT(w, n)	w ^= (1 << n)


#endif /* UTIL_H_ */