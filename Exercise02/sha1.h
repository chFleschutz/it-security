#ifndef SHA1_H
#define SHA1_H


typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned int uint16;
typedef signed int sint16;
typedef unsigned int uint8;
typedef signed int sint8;

typedef uint8 boolean;
#define FALSE (uint8)0
#define TRUE (uint8)1


typedef struct {
	uint32 *data;
	uint32 wordCount;
    uint32 usedBits;
} bitBlock;


/**
 * Reserves memory for a new bitBlock. 
 * The number of data-Elements is given by wordCount.
**/
extern bitBlock *initBitBlock(uint32 wordCount);
/**
 * Frees the memory that was allocated for the given bitBlock.
**/
extern void freeBitBlock(bitBlock *bb);
/**
 * Stores the character data given by the 0-terminated string
 * msg into a newly allocated bitBlock.
**/
extern bitBlock *forChars(const char *msg);
/**
* Restores a string that has previously been stored in 
* the given bitBlock using forChars.
**/
extern char *fromBitBlock(bitBlock * block);
/**
 * Outputs the content of the given bitBlock in Binary representation
 * on stdout.
**/
extern void printBitBlock(bitBlock *block);
/**
 * Prints a hexadecimal representation of the given uint32-Array to
 * stdout. Note that the size of a has to be given in the
 * respective size-parameter.
**/
extern void printWordArrayHex(uint32 *a, uint32 size);
/**
 * Prints a binary representation of the given uint32-Array to
 * stdout. Note that the size of a has to be given in the
 * respective size-parameter. This function can e.g. be used to
 * print a hash-value in the standard hexadecimal representation
 * (in which case size has to be 5 for sha-1).
**/
extern void printWordArrayBits(uint32 *a, uint32 size);

extern bitBlock *pad(bitBlock *block);
/**
 * Computes the hash-value belonging to the given message. Note
 * that the returned array is freshly allocated and has to be freed
 * as soon as it is no longer needed.
 * The hash-algorithm that is used here is sha-1. Hash values of
 * sha-1 always have 160 bits, hence the size
 * of the returned array is always 5 (since 5 * 32 Bits  = 160 Bits).
**/
extern uint32 *sha1(bitBlock *message);

/**
* Computes the hash-value belonging to the given message. 
* This function expects the message already to be padded (use
* the pad function for this). 
* Note that the returned array is freshly allocated and has to be freed
* as soon as it is no longer needed.
* The hash-algorithm that is used here is sha-1. Hash values of
* sha-1 always have 160 bits, hence the size
* of the returned array is always 5 (since 5 * 32 Bits  = 160 Bits).
**/
extern uint32 *sha1Faster(bitBlock *message);

#endif /* #ifndef SHA1_H */