#define _CRT_SECURE_NO_WARNINGS

#include "sha1.h"
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>


typedef struct {
    char** words;
    // number of words that can be stored in the words array
    uint32 capacity;
    // number of words that really are stored in the words array
    uint32 wordCount;
} wordVec;

/**
 * Initializes the word vector with a capacity of 16 words.
 */
wordVec *initWordVec(void) {
    wordVec *wv = (wordVec*)calloc(1, sizeof(wordVec));
    wv->wordCount = 0;
    wv->capacity = 16;
    wv->words = (char**)calloc(wv->capacity, sizeof(char*));
    return wv;
}

/**
 * Frees all memory used in the word vector including the 
 * memory allocated for the strings of the vector.
 */
void freeWordVec(wordVec *wv) {
    uint32 i;
    for (i = 0; i < wv->wordCount; i++) {
        free(wv->words[i]);
    }
    free(wv->words);
    free(wv);
}

/**
 * Doubles the capacity of the word vector.
 */
void doubleCapacity(wordVec *wv) {
    char** newWordsPointer = (char**)realloc(wv->words, wv->capacity * 2 * sizeof(char*));
    if (newWordsPointer != NULL) {
        wv->capacity = wv->capacity * 2;
        wv->words = newWordsPointer;
        return;
    }
    fprintf(stderr, "Could not double capacity of wordVec.");
    exit(1);
}

/**
 * Adds the given word to the word vector. The capacity of 
 * the vector is increased, if necessary. The word is not
 * copied, so do not free the memory occupied by word unless
 * you know what you are doing.
 */
void add(wordVec *wv, char* word) {
    if (wv->wordCount >= wv->capacity) doubleCapacity(wv);
    wv->words[wv->wordCount] = word;
    wv->wordCount++;
}


/**
 * Copies the content of the 0-terminated string line into
 * a new 0-terminated string that occupies exactly as much
 * memory as is needed to hold the content.
 */
char* copyLine(char* line) {
    uint32 count = 0;
    uint32 i;
    char* s;
    while (line[count] != 0) count++;
    count++;
    s = (char*)calloc(count, sizeof(char));
    for (i = 0; i < count; i++) {
        s[i] = line[i];
    }
    return s;
}


/**
 * Loads a list of passwords from the file implied by the
 * given fileName. This file has to contain a list of words
 * all terminated by a line-break. All of the words in the
 * list have to have at most 39 chars, otherwise this function
 * is prone to buffer-overflows. You have been warned.
 */
wordVec *loadPasswordList(char* fileName) {
    FILE *f;
    wordVec *wv = initWordVec();
    uint32 err;
    char* line = (char*)calloc(40, sizeof(char));
    f = fopen(fileName, "r");
    err = fscanf(f, "%s", line);
    while (err != EOF) {
        add(wv, copyLine(line));
        err = fscanf(f, "%s", line);
    }
    return wv;
}



void print(wordVec *wv) {
    uint32 i;
    printf("wordCount: %d", wv->wordCount);
    for (i = 0; i < wv->wordCount; i++) {
        printf("%s\n", wv->words[i]);
    }
}

/**
 * Returns TRUE if and only if the two given hashes equal.
 * The hashes have to be stored in uint32-arrays of size 5.
 */
boolean hashesEqual(uint32 *h1, uint32 *h2) {
    uint32 i;
    for (i = 0; i < 5; i++) {
        if (h1[i] != h2[i]) return FALSE;
    }
    return TRUE;
}


std::string bruteForceCrack(uint32* sha1Hash, const std::string& alphabet)
{
    std::string passwordGuess;
    int currentIndex = 0;

    // Check currentIndex permutations
    for (const auto& c : alphabet)
    {
        passwordGuess[currentIndex] = c;
        auto hash = sha1(forChars(passwordGuess.c_str()));
        if (hashesEqual(sha1Hash, hash))
            return passwordGuess;
    }

}

std::string bruteForceFixed(uint32* sha1Hash, const std::string& alphabet, int passwordLength)
{
    std::string passwordGuess;
    std::vector<int> currentIndex(passwordLength, 0);
    
    for (const auto& c : alphabet)
    {
		auto hash = sha1(forChars(passwordGuess.c_str()));
		if (hashesEqual(sha1Hash, hash))
			return passwordGuess;
	}   

}


int main()
{
    std::string capitalLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string smallerLetters = "abcdefghijklmnopqrstuvwxyz";
    std::string nums = "0123456789";
    std::string binary = "01";

    std::string password = "123";
    std::cout << "Original Password: " << "\t" << password << std::endl;

    auto hash = sha1(forChars(password.c_str()));
    std::cout << "Hashed Password: " << "\t";
    printWordArrayHex(hash, 5);

    auto bruteforced = bruteForceCrack(hash, nums);

    return 0;
}