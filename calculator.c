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
	if(!(p->end & 0x01)) p->end--;
	bitfield.length = (uint32_t) ((p->end - p->start) / (2*BIT_SIZE)) + 1;
	printf("Starting Thread %i : %i-%i BITFIELD=%i [sqrt(%i-%i) has %i Prime Numbers (i=%i)]\n",
	 p->id+1, p->start, p->end, bitfield.length, p->w_start, p->w_end, p->w_primecount, p->w_sum);
	writePrimesInArray(p->w_sum, p->w_start, p->w_end);

	bitfield.field = (uint64_t*) malloc(bitfield.length*BIT_SIZE);
	bfReset(&bitfield);

	pthread_barrier_wait(&barrier);
	calcWithSieve(p->start, p->end, &bitfield);
	//bfPrint(&bitfield);
#ifdef DEBUG
	p->primecount = bfCount0(&bitfield, (p->end - p->start), calcWithMod(p->start, p->end));
#else
	p->primecount = bfCount0(&bitfield, (p->end - p->start), 0);
#endif

	printf("Thread %i: (%i-%i) finished! %u Primes\n",p->id+1, p->start, p->end, p->primecount);
	free(bitfield.field);
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

uint32_t calcWithMod(uint32_t start, uint32_t end)
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

/* Sieve of Eratosthenes
*  it iterates through all the low primes and marks every
*	 multiple as a non-prime (bitset)
*/
void calcWithSieve(uint32_t start, uint32_t end, bitfield_t* bf)
{
	uint32_t i, currNum, offset;

	for(i = primesUntilSquare; i;)
	{
		offset = low_primes[--i];
		for(currNum = findStart(start, offset); currNum < end; currNum += offset*2)
		{
			//Set bit to indicate that this is NOT a prime!
			//the diff (curr - start) is ALWAYS even!
			bfSetbit(bf, (currNum - start) >> 1);
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

uint32_t bfCount0(bitfield_t* bf, uint32_t real_length, uint32_t debugArg)
{
	uint32_t i, result = 0, length_diff, real_length_div, sieveRes;
	for(i = bf->length; i;)
			result += BIT_SIZE - __builtin_popcountll(bf->field[--i]);

#ifdef DEBUG

	real_length_div = real_length >> 1; //(real_length & 0x01) ? ((real_length - 1) >> 1) : ((real_length - 2) >> 1);
	length_diff = BIT_SIZE * bf->length - real_length_div;
	//length_diff = BIT_SIZE * bf->length - real_length;
	sieveRes = result - length_diff;
	char vgC = (sieveRes < debugArg) ? '<' : '>';
	//if(sieveRes - debugArg)
		printf("real=%u div=%u used=%u ignore=%i erg=%u %c %u\n", real_length, real_length_div, (BIT_SIZE * bf->length), length_diff, sieveRes, vgC, debugArg);
	// TODO: Maybe the error is in the sieve
	return 0;

#else // Normal Code

	length_diff = BIT_SIZE * bf->length - (real_length >> 1);
	return result - length_diff;

#endif
}

void bfPrint(bitfield_t* bf)
{
	for(uint32_t i = bf->length; i;)
		printLong(bf->field[--i]);
}

//Sets a bit in the bitfield
void bfSetbit(bitfield_t* bf, uint32_t index)
{
	uint32_t arr_index, bit_index;
	arr_index = (index/BIT_SIZE);
	bit_index = index - arr_index * BIT_SIZE;
	bf->field[arr_index] |= (1L<<bit_index);
}

void bfReset(bitfield_t* bf)
{
	uint32_t i;
	for(i = bf->length; i;)
			bf->field[--i] = 0;
	bf->bitCnt = 1;			  //-> reset
	bf->bit = 0x01;
	bf->index = 0;

}
