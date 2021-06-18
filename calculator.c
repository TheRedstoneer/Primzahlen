/*
 * calculator.c
 *	Version vom 23.05.2021
 *  Created on: Nov 13, 2018
 *      Author: David Dinse
 */

#include "control.h"

void createPrimeArray(uint32_t end)
{


	uint32_t mod, i;
	unsigned short isMod;
	for(i = 3; i < end; i +=2)
	{
		for(mod = 3; mod*mod <=i; mod+=2)
			if(i % mod == 0)
			{
				isMod++;
				break;
			}
	}
}

void calcThread(void *arg)
{
	primeparam_t p;
	p = (primeparam_t) arg;
	bitfield_t bitfield;
	if(!(p->start & 0x01)) p->start++;
	bitfield.length = (uint32_t) ((p->end - p->start) / (2*BIT_SIZE)) + 1;
	printf("Starting Thread %i : %i-%i BITFIELD=%i [sqrt(%i-%i) has %i Prime Numbers (i=%i)]\n",
	 p->id+1, p->start, p->end, bitfield.length, p->w_start, p->w_end, p->w_primecount, p->w_sum);
	writePrimesInArray(p->w_sum, p->w_start, p->w_end);

	bitfield.field = (uint64_t*) malloc(bitfield.length*BIT_SIZE);
	bf_reset(&bitfield);
	p->primecount = bf_count_0(&bitfield, (p->end - p->start));
	printf("Thread %i: TEST %u = %u\n", p->id+1, p->primecount, bitfield.length*BIT_SIZE);

	printf("Thread %i: Written all low_primes into array!\n",p->id+1);
	/* 100.000 -> 5.000 bits / 10 threads (just uneven numbers)
		-> 5.000 / 64 = 79 fields
	*/

	pthread_barrier_wait(&barrier);
	calcWithSieve(p->start, p->end, &bitfield);
	p->primecount = bf_count_0(&bitfield, (p->end - p->start));
	//printLong(bitfield.field[0]);
	printf("Thread %i: (%i-%i) finished! %u Primes\n",p->id+1, p->start, p->end, p->primecount);
	free(bitfield.field);
}

/*void calcThreadDebug(void *arg)
{
	primeparam_t p;
	p = (primeparam_t) arg;
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

}*/
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

void calcWithSieve(uint32_t start, uint32_t end, bitfield_t* bf)
{
	uint32_t i, currNum, offset;

	for(i = primesUntilSquare; i;)
	{
		offset = low_primes[--i];
		for(currNum = findStart(start, offset); currNum < end; currNum += offset*2)
		{
			//Set bit to indicate that this is NOT a prime!
			//the diff (curr - start) is ALWAYS uneven!
			bf_setbit(bf, (currNum - start) >> 1);
		}
	}
}

uint32_t findStart(uint32_t start, uint32_t mod)
{
	// integer-division Start/Mod gets us the "real dividand" for this mod.
	// now we just multiply it again to get the new/modified start.
	uint32_t newStart = (uint32_t) (start/mod + 1) * mod;
	// if its even, add mod (which is uneven) again.
	if(!(newStart & 0x01)) newStart += mod;

	return newStart;
}

uint32_t bf_count_0(bitfield_t* bf, uint32_t real_length)
{
	uint32_t i, result = 0, length_diff;
	for(i = bf->length; i;)
			result += BIT_SIZE - __builtin_popcountll(bf->field[--i]);

	length_diff = BIT_SIZE * bf->length - (real_length & 0x01) ? ((real_length - 1) >> 1) : (real_length >> 1);
	//printf("real=%u used=%u -> ignoring %i bits!\n", real_length_uneven, (BIT_SIZE * bf->length), length_diff);
	return result;
}

//Sets a bit in the bitfield
void bf_setbit(bitfield_t* bf, uint32_t index)
{
	uint16_t arr_index, bit_index;
	arr_index = (index/BIT_SIZE);
	bit_index = index - arr_index * BIT_SIZE;
	bf->field[arr_index] |= (1L<<bit_index);
}

void bf_reset(bitfield_t* bf)
{
	uint32_t i;
	for(i = bf->length; i;)
			bf->field[--i] = 0;
	bf->bitCnt = 1;			  //-> reset
	bf->bit = 0x01;
	bf->index = 0;

}
