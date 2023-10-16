#include "largeInt.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>


/**
 ** Returns the number of leading zeroes of the given argument.
 **
 ** A leading zero is a binary digit of the given arg that is set
 ** to 0 and to the left of which only appear 0's.
 **/
uint8 GetNumberOfLeadingZeroes(uint32 arg)
{
	uint32 mask = 0x80000000U;
	uint8 count = 0;

	while (((arg & mask) == 0) && (count < 32))
	{
		mask = mask >> 1;
		count++;
	}
	return count;
}


/**
 ** \brief Checks for leading zeroes and, based on the number of these,
 **        computes b->usedWords and b->bitSize.
 **
 ** \param[in] b The BigInt, for which to compute the usage values.
 **/
void RecomputeUsageVariables(LargeInt* b)
{
	uint32 i;
	uint32* dataPointer;

	i = b->wordSize - 1;
	dataPointer = &(b->data[i]);

	while (((*dataPointer) == 0) && (i > 0))
	{
		i--;
		dataPointer--;
	}

	b->bitSize = 32 - GetNumberOfLeadingZeroes((*dataPointer)) + (i * BITSPERWORD);
	if (b->bitSize == 0)
	{
		b->usedWords = 0;
	}
	else
	{
		b->usedWords = i + 1;
	}
}

/**
 ** Returns TRUE if and only if the given LargeInt is even.
 **/
boolean IsEven(const LargeInt* b)
{
	uint32 mask = 1;
	return 1 - (b->data[0] & mask);
}

/**
 ** Returns TRUE if and only if the given LargeInt is odd.
 **/
boolean IsOdd(const LargeInt* b)
{
	return 1 - IsEven(b);
}

/**
 ** Initializes a new LargeInt with the given integer value.
 **
 ** \param[in] value The value that is to be passed to the new LargeInt.
 ** \param[wordSize] The number of 32-Bit-words that shall used in the new LargeInt.
 ** \return A LargeInt that has been initialized with the given value.
 **/
LargeInt* InitLargeIntWithUint32(uint32 value, uint8 wordSize)
{
	LargeInt* x = (LargeInt*)calloc(1, sizeof(LargeInt));
	x->data = (uint32*)calloc(wordSize, sizeof(uint32));
	x->wordSize = wordSize;

	if (value == 0)
	{
		x->usedWords = 0;
		x->bitSize = 0;
	}
	else
	{
		x->usedWords = 0;
		x->bitSize = 32 - GetNumberOfLeadingZeroes(value);
		while (value > 0)
		{
			x->data[x->usedWords] = value & STANDARD_USEBIT_MASK;
			x->usedWords++;
			value = value >> BITSPERWORD;
		}
	}
	return x;
}


/**
 * Frees the memory of the given LargeInt.
 */
void freeLargeInt(LargeInt* x)
{
	free(x->data);
	free(x);
}


/**
 ** \brief Adds the two given summands and returns the result.
 **
 ** This algorithm treats the arguments as if they were both
 ** positive, i.e. the sign of the integers is ignored.
 **
 ** \param[in] s1 The first summand.
 ** \param[in] s2 The second summand.
 ** \result The sum of s1 and s2. The word size of the result
 **         is exactly as large as needed to hold the sum.
 **
 **/
LargeInt* Add(LargeInt* s1, LargeInt* s2)
{
	int largerWordSize = std::max(s1->usedWords, s2->usedWords);
	LargeInt* result = InitLargeIntWithUint32(0, largerWordSize);

	int bits = 0;
	uint32 overflow = 0;
	for (int i = 0; i < largerWordSize; i++)
	{
		// Add s1, s2 and overflow (check if s1/s2 still have data)
		uint32 addResult = overflow;
		if (i < s1->usedWords)
			addResult += s1->data[i];
		if (i < s2->usedWords)
			addResult += s2->data[i];

		// Make sure to only save the usebits
		result->data[i] = addResult & STANDARD_USEBIT_MASK;

		// Calculate overflow
		overflow = addResult & STANDARD_CALCBIT_MASK;
		overflow = overflow >> BITSPERWORD;
		bits += BITSPERWORD;
	}
	//Todo: check for overflow on last word

	// Check bits
	auto zeros = BITSPERWORD - (32 - GetNumberOfLeadingZeroes(result->data[largerWordSize - 1]));
	bits -= zeros;

	result->usedWords = largerWordSize;
	result->bitSize = bits;
	return result;
}


void printLargeInt(LargeInt* x)
{
	int i = x->bitSize - 1;
	while (i >= 0)
	{
		int wordIndex = i / BITSPERWORD;
		int bitIndex = i % BITSPERWORD;
		uint32 testBit = 1 << bitIndex;
		if ((x->data[wordIndex] & testBit) != 0)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		i--;
	}
	printf("\n");
}



// Verstehen Sie die untige main-Funktion bitte als Anstoß zum 
// Testen Ihres Codes. Fügen Sie weitere, sinnvolle Tests hinzu!
int main()
{
	LargeInt* x = InitLargeIntWithUint32(70000, 5);
	x->bitSize++; // better checking in console
	LargeInt* y = InitLargeIntWithUint32(80001, 5);
	y->bitSize++; // better checking in console
	printLargeInt(x);
	printLargeInt(y);
	LargeInt* z = Add(x, y);
	printLargeInt(z);
	return 0;
}
