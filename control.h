/*
 * control.h
 *	Version vom 5.12. 10:50
 *  Created on: Nov 13, 2018
 *      Author: tiws17g11
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void createPrimeArray(__uint32_t end);
void calcThread(void *args);
void calcThreadDebug(void *args);
__uint64_t calcWithMod(__uint32_t start, __uint32_t end);
__uint64_t calcWithSieve(__uint32_t start, __uint32_t end);
void writePrimesInArray(__uint32_t index, __uint32_t start, __uint32_t end);
void printLong(long x);
__uint32_t findStart(__uint32_t start, __uint32_t mod);
void printLowPrimes();

#define BIT_SIZE 64
#define SetBit(zahl,bit)	( zahl[bit/BIT_SIZE] |= ((__uint64_t)1 << (bit%BIT_SIZE)) )
#define ClearBit(zahl,bit)	( zahl[bit/BIT_SIZE] &= ~((__uint64_t)1 << (bit%BIT_SIZE)) )
#define RESET_FIELD(field,length) for(i = length; i > 0; i--) field[i-1] = 0

struct Parameter
{
	__uint32_t w_start;
	__uint32_t w_end;
	__uint32_t w_primecount;
	__uint32_t w_sum;
	__uint32_t start;
	__uint32_t end;
	__uint16_t id;
};

// Globale Variablen
volatile __uint64_t* bitfield;
volatile __uint64_t* w_bitfield;
volatile __uint32_t* low_primes;
__uint32_t primesUntilSqare;
pthread_barrier_t barrier;
#endif /* CONTROL_H_ */
