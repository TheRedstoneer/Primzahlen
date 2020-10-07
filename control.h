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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void createPrimeArray(uint32_t end);
void calcThread(void *args);
void calcThreadDebug(void *args);
uint64_t calcWithMod(uint32_t start, uint32_t end);
uint64_t calcWithSieve(uint32_t start, uint32_t end, uint32_t arrIndex);
void writePrimesInArray(uint32_t index, uint32_t start, uint32_t end);
void printLong(long x);
uint32_t findStart(uint32_t start, uint32_t mod);
void printLowPrimes();

#define BIT_SIZE 64
#define SetBit(zahl,bit)	( zahl[bit/BIT_SIZE] |= ((uint64_t)1 << (bit%BIT_SIZE)) )
#define ClearBit(zahl,bit)	( zahl[bit/BIT_SIZE] &= ~((uint64_t)1 << (bit%BIT_SIZE)) )
#define RESET_FIELD(field,length) for(i = length; i > 0; i--) field[i-1] = 0

struct Parameter
{
	uint32_t w_start;
	uint32_t w_end;
	uint32_t w_primecount;
	uint32_t w_sum;
	uint32_t start;
	uint32_t end;
	uint16_t id;
};

// Globale Variablen
volatile uint64_t* bitfield;
volatile uint64_t* w_bitfield;
volatile uint32_t* low_primes;
uint32_t primesUntilSqare, bits_pT;
pthread_barrier_t barrier;
#endif /* CONTROL_H_ */
