/*
 * calculator.c
 *	Version vom 5.12. 10:50
 *  Created on: Nov 13, 2018
 *      Author: tiws17g11
 */

#include "control.h"

void createPrimeArray(uint32_t end)
{


	uint32_t mod, i, j;
	unsigned short isMod;
	for(i = 3; i < end; i +=2)
	{
		for(mod = 3; mod*mod <=i; mod+=2)
			if(i % mod == 0)
			{
				isMod++;
				break;
			}
			//if(!isMod)

	}
}

void calcThread(void *arg)
{
	struct Parameter *p;
	p = (struct Parameter*) arg;
	printf("Starting Thread %i : %i-%i 	[and sqrt(%i-%i) with %i Prime numbers (i=%i)]\n",
	 p->id+1, p->start, p->end, p->w_start, p->w_end, p->w_primecount, p->w_sum);
	writePrimesInArray(p->w_sum, p->w_start, p->w_end);

	printf("Thread %i: Written all low_primes into array!\n",p->id+1);
	/* 100.000 -> 5.000 bits / 10 threads (just uneven numbers)
		-> 5.000 / 64 = 79 fields
	*/

	pthread_barrier_wait(&barrier);
	calcWithSieve(p->start, p->end,bits_pT*p->id);
	printf("Thread %i: (%i-%i) finished!\n",p->id+1,p->start,p->end);
}

void calcThreadDebug(void *arg)
{
	struct Parameter *p;
	p = (struct Parameter*) arg;
	uint64_t erg1, erg2;
	printf("Starting Thread %i : %i-%i 	[and sqrt(%i-%i) with %i Prime numbers (i=%i)]\n",
	 p->id+1, p->start, p->end, p->w_start, p->w_end, p->w_primecount, p->w_sum);
	writePrimesInArray(p->w_sum, p->w_start, p->w_end);

	printf("Thread %i: Written all low_primes into array!\n",p->id+1);
	pthread_barrier_wait(&barrier);
	erg1 = calcWithSieve(p->start, p->end,bits_pT*p->id);
	erg2 = calcWithMod(p->start, p->end);
	if(erg1 == erg2)
	printf("Thread %i: (%i-%i) has [%li] Primenumbers!\n",p->id+1,p->start,p->end,erg2);
	else
	{
		if(erg1 > erg2)		erg2 = erg1 - erg2;
		else erg2 -= erg1;
		printf("Thread %i: (%i-%i) has %li / %li Primenumbers!\n",p->id+1,p->start,p->end,erg1,erg2);
	}

}
 //
void writePrimesInArray(uint32_t index, uint32_t start, uint32_t end)
{
	uint32_t i, mod;
	unsigned short isMod;

	if(!(start % 2)) start++; //if even, start with +1
	for(i = start; i < end+1; i+=2)
	{
		isMod = 0;
		for(mod = 3; mod*mod <=i; mod+=2)
			if(i % mod == 0)
			{
				isMod++;
				break;
			}
		if(!isMod)
		{
			low_primes[index] = i;
			index++;
		}
	}
}

uint64_t calcWithMod(uint32_t start, uint32_t end)
{
	uint64_t i, erg = 0;
	uint32_t mod;
	unsigned short isMod;
  if(!(start % 2)) start++;
	for(i = start; i < end+1; i+=2)
	{
		isMod = 0;
		for(mod = 3; mod*mod <=i; mod+=2)
			if(i % mod == 0)
			{
				isMod++;
				break;
			}
		if(!isMod)
		{
			erg++;
		}
	}
	return erg;
}

uint64_t calcWithSieve(uint32_t start, uint32_t end, uint32_t arrIndex)
{
	uint32_t* countArr = (uint32_t*) malloc(primesUntilSqare * sizeof(countArr));
	uint32_t j, i;
	uint64_t erg = 0, bit = 1, bitC = 1;
	bool isPrime;
	if(!(start % 2)) start++; //if even, start with +1
	for(i = primesUntilSqare; i; i--)
	{// (p - [(findNext(start,p)-start)/2]%p)%p
		 countArr[i-1] = findStart(start-2, low_primes[i-1]);	//setup counting array
		 //printf("%i starts on %i\n", low_primes[i-1], countArr[i-1]);
	}


	//go through every uneven number
	for(i = start; i < end; i+=2)
	{
		isPrime = 1; //lets say it's a prime

		//now go through every low prime
		for(j = 0; j < primesUntilSqare; j++)
		{
			//it's not a prime if the countArr is 0 -> divide-able
			if(isPrime && !countArr[j]) //array[] = 0 AND isPrime = 1
			{
				isPrime = false;
				//printf("n=%i (%i/%i)\n",i,countArr[j],low_primes[j]);
			}
			countArr[j]++;
			//if counter reached end the low prime -> reset
			if(countArr[j] >= low_primes[j]) countArr[j] -= low_primes[j];

			//if(!(countArr[j-1] - low_primes[j-1])) countArr[j-1] = 0;

		}
		if(isPrime)
		{
			erg++; //debug
			//set bit!
			bitfield[arrIndex] |= bit;
			//printf("%i\n",i);
		}

		bit = bit << 1; //shift ->
		bitC++;
		if(bitC == 64)
		{									//test for overflow (64 bits)
			bitC = 1;			  //-> reset
			bit = 1;
			arrIndex++;
		}
	}
return erg;
}

uint32_t findStart(uint32_t start, uint32_t mod)
{
	uint32_t x = start;
	while(x % mod) x+= 2; //find next divider
				//7 - ((21- 13	)/2)	% 7 = 3
	return mod- ((x - start)/2) % mod;
}
