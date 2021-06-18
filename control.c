/*
 * main.c
 *	Version vom 7.01.19 10:20
 *  Created on: Nov 13, 2018
 *      Author: David Dinse
 */

#include "control.h"


int main(int argc, char *argv[]) // Primzahl Threads
{
	uint32_t prime_number, n_threads, i, wurzel, pre_calc;
	uint32_t mainResult;
	bool debug = false;
	primeparam_t param;

	if(argc < 3 || argc > 4)
	{
		printf("USAGE: ./%s <MAX NUMBER> <THREAD COUNT> [debug]\n", argv[0]);
		return(-1);
	}
	else
	{
		if(argc == 4) debug = true;
		prime_number = (uint32_t) strtol(argv[1], NULL, 10);
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

	wurzel = sqrt(prime_number)+1;
	primesUntilSquare = calcWithMod(3, wurzel); //get prime number count til sqrt (without 2)
	printf("Sqrt=%i Primes until Square=%i\n", wurzel, primesUntilSquare);
	low_primes = (uint32_t*) malloc(32*primesUntilSquare); //create array

	//store Param structs
	param = (primeparam_t) malloc(n_threads*sizeof(struct _PARAMS));

	mainResult = (prime_number-wurzel) / n_threads; //calculate Numbers per Thread
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
			param[i].end = prime_number;		//set real end for last thread
		 	param[i].w_end = wurzel;		//set real sqrt end for last thread
		}
		//printf("Thread %i Start: %i End: %i\n",i+1,param[i].start,param[i].end);
		param[i].w_primecount = calcWithMod(param[i].w_start,param[i].w_end);
		if(i) param[i].w_sum += param[i-1].w_primecount + param[i-1].w_sum;
		param[i].id = i; 									//set ID
		param[i].primecount = 0;
		pthread_create(&param[i].tid, NULL, (void*)calcThread, (void*)&param[i]);

	}
	pthread_barrier_wait(&barrier);
	if(debug) printf("- Debug -");
	printf("Continue...\n");

	mainResult = primesUntilSquare + 1;
	//Threads haben ihren bereich fertig gerechnet
	//threads einsammeln
	for(i = 0; i < n_threads; i++)
	{
		if(pthread_join(param[i].tid, NULL))
		{
			printf("Could not join Thread %i\n", i);
			return -1;
		}
		mainResult += param[i].primecount;
	}

	printf("\nThere are %i Primenumbers from 0-%i\n", mainResult, prime_number);
	free(low_primes);
	free(param);
	return 0;
}

void printLong(uint64_t x)
{
	uint64_t i;
	for(i = 0x8000000000000000; i; i = i >> 1)
	{
		if(i & x)
			printf("1");
		else
			printf("0");

	}
	printf("\n");
}

void printLowPrimes()
{
	int i;
	for(i = 0; i < primesUntilSquare; i++)
		printf("%i ",low_primes[i]);
	printf("\n");
}
