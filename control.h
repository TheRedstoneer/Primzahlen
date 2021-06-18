/*
 * control.h
 *	Version vom 5.12. 10:50
 *  Created on: Nov 13, 2018
 *      Author: tiws17g11
 */

#ifndef CONTROL_H_
#define CONTROL_H_

//#define DEBUG

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct _PARAMS
{
	uint16_t id;
	pthread_t tid;
	uint32_t start;
	uint32_t end;
	uint32_t primecount;

	uint32_t w_start;
	uint32_t w_end;
	uint32_t w_primecount;
	uint32_t w_sum;
} *primeparam_t;

typedef struct _BITFIELD
{
	uint64_t* field;
	uint64_t bit;
	uint32_t bitCnt, index, length;
} bitfield_t;

void createPrimeArray(uint32_t end);
void calcThread(void *args);
uint32_t calcWithMod(uint32_t start, uint32_t end);
void calcWithSieve(uint32_t start, uint32_t end, bitfield_t* bf);
void writePrimesInArray(uint32_t index, uint32_t start, uint32_t end);
void printLong(uint64_t x);
uint32_t findStart(uint32_t start, uint32_t mod);
void printLowPrimes();
uint32_t bfCount0(bitfield_t* bf, uint32_t real_length, uint32_t debugArg);
void bfSetbit(bitfield_t* bf, uint32_t index);
void bfPrint(bitfield_t* bf);
void bfReset(bitfield_t* bf);

#define BIT_SIZE 64

// Globale Variablen
uint32_t* low_primes;
uint32_t primesUntilSquare, bits_pT;
pthread_barrier_t barrier;
#endif /* CONTROL_H_ */
