/*
 * main.c
 *	Version vom 7.01.19 10:20
 *  Created on: Nov 13, 2018
 *      Author: David Dinse
 */

#include "control.h"


int main(int argc, char *argv[]) // Primzahl Threads
{
	pthread_t* thread_id;
	int* singleResult;
	uint32_t primzahl, n_threads, i, bitlen, w_bitlen, wurzel, pre_calc;
	uint32_t mainResult;
	bool debug = false;
	struct Parameter* param;

	if(argc < 3 || argc > 4)
	{
		printf("USAGE: ./Primzahlen <MAX NUMBER> <THREAD COUNT> [debug]\n");
		return(-1);

	}
	else
	{
		if(argc == 4) debug = true;
		primzahl = (uint32_t) strtol(argv[1], NULL, 10);
		n_threads = (uint32_t) strtol(argv[2], NULL, 10);
		if(n_threads > 32)
		{
			printf("Too many threads. max = 32\n");
			return(-1);
		}
	}

/****************************************/
	if(pthread_barrier_init(&barrier, NULL, n_threads+1))
	{
		printf("Could not create thread barrier.");
		return -1;
	}
// Array for SQRT Bit Array
	wurzel = sqrt(primzahl)+1;
	//w_bitlen = (int)(wurzel / BIT_SIZE+1);
	//w_bitfield = (uint64_t*) malloc(w_bitlen*BIT_SIZE);
	//RESET_FIELD(w_bitfield,w_bitlen);

	primesUntilSqare = calcWithMod(3,wurzel); //get prime number count til sqrt (without 2)
	printf("Wurzel=%i Anzahl Primzahlen bis zur Wurzel=%i\n",wurzel,primesUntilSqare);
	low_primes = (uint32_t*) malloc(32*primesUntilSqare); //create array

// Array for Bit Array
// (prime_max - sqrt) / 2 (uneven) / 64
	bitlen = (uint32_t) ((primzahl-wurzel) / (2*BIT_SIZE) +n_threads);
	bitfield = (uint64_t*) malloc(bitlen*BIT_SIZE);
	bits_pT = (uint32_t) bitlen / n_threads + 1;
	RESET_FIELD(bitfield,bitlen);
	printf("Created %i Threads! (%i fields, %i/thread)\n",n_threads, bitlen, bits_pT );

	thread_id = (pthread_t*) malloc(n_threads*sizeof(pthread_t)); // store all thread IDs
	param = (struct Parameter*) malloc(n_threads*sizeof(struct Parameter)); //store Param structs

	mainResult = (primzahl-wurzel) / n_threads; //calculate Numbers per Thread
	pre_calc = (wurzel-3) / n_threads;
	param[0].start = wurzel+1;
	param[0].w_start = 3;
	param[0].w_sum = 0;

	for(i = 0; i < n_threads; i++)
	{
		if(i) param[i].start = param[i-1].end + 1; 			//set start
		if(i) param[i].w_start = param[i-1].w_end + 1; 			//set sqrt start
		param[i].end = param[i].start + mainResult;		 	//set end
		param[i].w_end = param[i].w_start + pre_calc;		 	//set sqrt end
		if(i == n_threads-1)
		{
			param[i].end = primzahl;		//set real end for last thread
		 	param[i].w_end = wurzel;		//set real sqrt end for last thread
		}
		//printf("Thread %i Start: %i End: %i\n",i+1,param[i].start,param[i].end);
		param[i].w_primecount = calcWithMod(param[i].w_start,param[i].w_end);
		if(i) param[i].w_sum += param[i-1].w_primecount + param[i-1].w_sum;
		param[i].id = i; 									//set ID
		if(debug)
			pthread_create(&thread_id[i], NULL, (void*)calcThreadDebug, (void*)&param[i]);
		else
			pthread_create(&thread_id[i], NULL, (void*)calcThread, (void*)&param[i]);

	}
	pthread_barrier_wait(&barrier);
	if(debug) printf("- Debug -");
	printf("Continue...\n");

	//Threads haben ihren bereich fertig gerechnet
	//threads einsammeln
	for(i = 0; i < n_threads; i++)
	{
		if(pthread_join(thread_id[i],NULL))
		{
			printf("Could not join Thread %i\n", i);
			return -1;
		}
	}
	if(debug) return 0;
	//printLowPrimes();
	mainResult = primesUntilSqare + 1;
	for(i = bitlen; i > 0; i--)
	{
		//printLong(bitfield[i-1]);
		mainResult += __builtin_popcountll(bitfield[i-1]);
		//mainResult += bitfield[i-1];

	}

	printf("\nErgebnis: %i\n", mainResult);
	return 0;
}

void printLong(long x)
{
	int i;
	for(i = 32; i >= 0 ; i--)
	{
		if((x&(1 << i%64)) == 0)
			printf("0");
		else
			printf("1");

	}
	printf("\n");
}

void printLowPrimes()
{
	int i;
	for(i = 0; i < primesUntilSqare; i++)
		printf("%i ",low_primes[i]);
	printf("\n");
}
