#include "largeInt.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>

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
	auto largerWordSize = std::max(s1->usedWords, s2->usedWords) + 1;
	LargeInt* result = InitLargeIntWithUint32(0, largerWordSize);

	uint32 bits = 0;
	uint32 overflow = 0;
	for (uint32 i = 0; i < largerWordSize; i++)
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
		overflow = addResult >> BITSPERWORD;
		bits += BITSPERWORD;
	}

	// Set bits to exclude leading zeroes
	bits -= BITSPERWORD - (32 - GetNumberOfLeadingZeroes(result->data[largerWordSize - 1]));

	result->usedWords = largerWordSize;
	result->bitSize = bits;
	return result;
}

/// @brief Adds num to the LargeInt beginning at index
/// @param base LargeInt num should be added to
/// @param num Number to be added to base
/// @param index Starting word for adding
/// @return Same ptr to base
LargeInt* Add(LargeInt* base, uint32 num, uint32 index)
{
	while (num > 0)
	{
		assert(index < base->wordSize && "Index out of Range make sure LargeInt can hold the addition");

		uint32 addResult = base->data[index] + (num & STANDARD_USEBIT_MASK);
		base->data[index] = addResult & STANDARD_USEBIT_MASK;	// Save usebits
		num += addResult & STANDARD_CALCBIT_MASK;				// Add overflow
		num = num >> BITSPERWORD;								// Shift to next word
		index++;
	}
	return base;
}

/// @brief Multiplies m1 and m2 and returns the result
/// @param m1 Pointer to the first operand
/// @param m2 Pointer to the second operand
/// @return Pointer to the new LargeInt with the multiplication result
LargeInt* Multiply(const LargeInt* m1, const LargeInt* m2)
{
	auto result = InitLargeIntWithUint32(0, m1->usedWords + m2->usedWords);

	for (uint32 i = 0; i < m1->usedWords; i++)
	{
		for (uint32 j = 0; j < m2->usedWords; j++)
		{
			uint32 mulRes = m1->data[i] * m2->data[j];
			Add(result, mulRes, i + j);
		}
	}

	RecomputeUsageVariables(result);
	return result;
}

void printLargeInt(LargeInt* x)
{
	int i = x->bitSize - 1;
	std::string result{};
	while (i >= 0)
	{
		int wordIndex = i / BITSPERWORD;
		int bitIndex = i % BITSPERWORD;
		uint32 testBit = 1 << bitIndex;
		if ((x->data[wordIndex] & testBit) != 0)
		{
			result.append("1");
		}
		else
		{
			result.append("0");
		}
		i--;
	}
	std::cout << result << "\n";
}

/// @brief Prints the LargeInts visualized like an operation for easy result checking
void visualizeOperation(LargeInt* first, LargeInt* second, LargeInt* result, char operation)
{
	int width = std::max({ first->bitSize, second->bitSize, result->bitSize }) + 2;

	std::cout << std::setw(width) << std::right;
	printLargeInt(first);

	std::cout << std::left << operation << std::setw(width - 1) << std::right;
	printLargeInt(second);

	std::cout << std::setw(width) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
	std::cout << std::setw(width) << std::right;
	printLargeInt(result);
}


// Verstehen Sie die untige main-Funktion bitte als Anstoß zum 
// Testen Ihres Codes. Fügen Sie weitere, sinnvolle Tests hinzu!
int main()
{
	auto x = InitLargeIntWithUint32(1023, 2);
	auto y = InitLargeIntWithUint32(1023, 2);
	auto z = Multiply(x, y);
	
	visualizeOperation(x, y, z, '*');
	
	freeLargeInt(x);
	freeLargeInt(y);
	freeLargeInt(z);
	return 0;
}
