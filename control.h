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

void createPrimeArray(unsigned int end);
void calcThread1(void *args);
void calcThread2(void *args);
__uint64_t calcWithMod(int start, int end);
__uint64_t calcWithSieve(int start, int end);
void writePrimesInArray(int index, int start, int end);
void printLong(long x);
int findNext(int start, int mod);
void printLowPrimes();

#define BIT_SIZE 64
#define SetBit(zahl,bit)	( zahl[bit/BIT_SIZE] |= ((__uint64_t)1 << (bit%BIT_SIZE)) )
#define ClearBit(zahl,bit)	( zahl[bit/BIT_SIZE] &= ~((__uint64_t)1 << (bit%BIT_SIZE)) )

struct Parameter
{
	int w_start;
	int w_end;
	int w_primecount;
	int w_sum;
	int start;
	int end;
	int id;
};

// Globale Variablen
volatile __uint64_t* bitfield;
volatile __uint64_t* w_bitfield;
volatile __uint32_t* low_primes;
__uint16_t primesUntilSqare;
pthread_barrier_t barrier;
#endif /* CONTROL_H_ */
