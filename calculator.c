/*
 * calculator.c
 *	Version vom 5.12. 10:50
 *  Created on: Nov 13, 2018
 *      Author: tiws17g11
 */

#include "control.h"

void createPrimeArray(unsigned int end)
{


	int mod, i, j;
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

void calcThread1(void *arg)
{
	struct Parameter *p;
	p = (struct Parameter*) arg;
	printf("Starting Thread %i : %i-%i\n", p->id+1, p->start, p->end);
}

void calcThread2(void *arg)
{
	struct Parameter *p;
	p = (struct Parameter*) arg;
	__uint64_t erg1, erg2;
	printf("Starting Thread %i : %i-%i 	[and sqrt(%i-%i) with %i Prime numbers (i=%i)]\n",
	 p->id+1, p->start, p->end, p->w_start, p->w_end, p->w_primecount, p->w_sum);
	writePrimesInArray(p->w_sum, p->w_start, p->w_end);

	printf("Thread %i: Written all low_primes into array!\n",p->id+1);
	pthread_barrier_wait(&barrier);
	erg1 = calcWithSieve(p->start, p->end);
	erg2 = calcWithMod(p->start, p->end);
	if(erg1 == erg2)
	printf("Thread %i: (%i-%i) has [%li] Primenumbers!\n",p->id+1,p->start,p->end,erg2);
	else
	printf("Thread %i: (%i-%i) has %li / %li Primenumbers!\n",p->id+1,p->start,p->end,erg2,erg1-erg2);
	//bitfield[p->id] = erg;
}
 //
void writePrimesInArray(int index, int start, int end)
{
	int i, mod;
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

__uint64_t calcWithMod(int start, int end)
{
	__uint64_t i, erg = 0;
	int mod;
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

__uint64_t calcWithSieve(int start, int end)
{
	__uint16_t* countArr = (__uint16_t*) malloc(primesUntilSqare * sizeof(countArr));
	unsigned int j, i;
	__uint64_t erg = 0;
	__uint8_t isPrime;
	//unsigned int tempErgCount = 0;
	for(i = primesUntilSqare; i > 0; i--)
	{// (p - [(findNext(start,p)-start)/2]%p)%p
		 countArr[i-1] = (low_primes[i-1] - ((findNext(start, low_primes[i-1])-start)/2)%low_primes[i-1])%low_primes[i-1]; //reset counting array
		 printf("%i starts on %i (%i)\n", low_primes[i-1], countArr[i-1], findNext(start, low_primes[i-1]));
	 }

  if(!(start % 2)) start++; //if even, start with +1
	for(i = start; i < end+1; i+=2)
	{
		isPrime = 1;
		for(j = 0; j < primesUntilSqare; j++)
		{
			if(!countArr[j] && isPrime) //array[] = 0 AND isPrime = 1
			{
				isPrime = 0; //no prime
				//printf("n=%i (%i/%i)\n",i,countArr[j],low_primes[j]);
			}
			countArr[j]++;
			//if counter reached end from counter
			if(countArr[j] == low_primes[j]) countArr[j] = 0;

		}
		if(isPrime)
		{
			erg++;
			//SetBit(bitfield,i); //set prime
			//printf("->%i\n",i);
		}
	}
return erg;
}

int findNext(__uint16_t start, __uint16_t mod)
{
	__uint16_t x = start;
	while(x % mod) x+= 2;

	return mod-(x-start)%mod;
}
