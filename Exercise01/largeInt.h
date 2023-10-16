#pragma once

typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned int uint16;
typedef signed int sint16;
typedef unsigned int uint8;
typedef signed int sint8;

typedef uint8 boolean;

#define FALSE (uint8)0
#define TRUE (uint8)1

#define BITSPERWORD (uint32)5U
#define WORD_RADIX (uint32)(1 << BITSPERWORD)
#define STANDARD_USEBIT_MASK  (uint32)(WORD_RADIX - 1)
#define STANDARD_CALCBIT_MASK (uint32)(0xFFFFFFFFU ^ STANDARD_USEBIT_MASK)


/**
 ** LargeInt
 ** A structure to hold the data of a large integer.
 **/
/**
 ** data
 **        An array to store the actual data of the integer. It
 **        is stored as follows: data[0] contains the least
 **        significant digit, data[usedWords-1] holds the most
 **        significant digit. Each digit is stored in little-endian
 **        format. For each digit, BITSPERWORD bits are
 **        used to store information about the integer. The
 **        remaining bits are reserved for computations.
 **/
/**
 ** bitSize
 **        Contains the number of bits up to the leftmost
 **        bit that is set to one. If there is no bit
 **        that is set to one, then this value is set to 0.
 **/
/**
 ** wordSize
 **        The number of uint32 words in the data array. This value
 **        should be set once, when the BigInt is initialized and
 **        never be changed until it is destroyed.
 **/
/**
 ** usedWords
 **        The number of uint32 words in the data array
 **        which actually hold information about the integer.
 **        This means, that this value is wordSize minus the
 **        number of leading zero-words.
 **        If all words in the data array are set to 0, the
 **        value of this variable is 0.
 **/
typedef struct
{
	uint32* data;
	uint32 bitSize;
	uint32 wordSize;
	uint32 usedWords;
} LargeInt;

extern boolean IsEven(const LargeInt* b);
extern boolean IsOdd(const LargeInt* b);
extern LargeInt* InitLargeIntWithUint32(uint32 value, uint8 wordSize);
extern LargeInt* Add(LargeInt* s1, LargeInt* s2);
