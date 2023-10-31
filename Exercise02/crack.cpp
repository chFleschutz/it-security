#define _CRT_SECURE_NO_WARNINGS

#include "sha1.h"
#include <stdlib.h>
#include <stdio.h>

#include <cassert>
#include <chrono>
#include <fstream>
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
wordVec* initWordVec(void) {
	wordVec* wv = (wordVec*)calloc(1, sizeof(wordVec));
	wv->wordCount = 0;
	wv->capacity = 16;
	wv->words = (char**)calloc(wv->capacity, sizeof(char*));
	return wv;
}

/**
 * Frees all memory used in the word vector including the
 * memory allocated for the strings of the vector.
 */
void freeWordVec(wordVec* wv) {
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
void doubleCapacity(wordVec* wv) {
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
void add(wordVec* wv, char* word) {
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
wordVec* loadPasswordList(char* fileName) {
	FILE* f;
	wordVec* wv = initWordVec();
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



void print(wordVec* wv) {
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
boolean hashesEqual(uint32* h1, uint32* h2) {
	uint32 i;
	for (i = 0; i < 5; i++) {
		if (h1[i] != h2[i]) return FALSE;
	}
	return TRUE;
}

bool CompareToHash(const std::string& password, uint32* sha1Hash)
{
	auto msgBitBlock = forChars(password.c_str());
	auto msgHash = sha1(msgBitBlock);
	auto result = hashesEqual(msgHash, sha1Hash);

	freeBitBlock(msgBitBlock);
	free(msgHash);

	return result;
}

uint32* convert160BitHash(const std::string& hexValue)
{
	assert(hexValue.size() == 40 && "Hexadecimal value has to consist of 40 characters to get 160 bits in integers");

	uint32* hash = (uint32*)calloc(5, sizeof(uint32)); // *hust*
	for (int i = 0; i < 5; i++)
	{
		hash[i] = std::stoul(hexValue.substr(i * 8, 8), nullptr, 16);
	}

	return hash;
}

uint32* toHash(const std::string& password)
{
	auto msgBitBlock = forChars(password.c_str());
	auto hash = sha1(msgBitBlock);

	freeBitBlock(msgBitBlock);
	return hash;
}

std::string bruteForceCrack(uint32* sha1Hash, const std::string& alphabet)
{
	std::string passwordGuess = std::string(1, alphabet[0]);
	std::vector<int> alphabetIndices = { 0 };

	while (true)
	{
		// Test all chars on first pos
		for (const auto& c : alphabet)
		{
			passwordGuess.front() = c;
			if (CompareToHash(passwordGuess, sha1Hash))
				return passwordGuess;
		}

		// Advance next char and handly possible overflow
		int charPos = 1;
		for (; charPos < passwordGuess.size(); charPos++)
		{
			alphabetIndices[charPos]++;
			if (alphabetIndices[charPos] < alphabet.size())
			{
				passwordGuess[charPos] = alphabet[alphabetIndices[charPos]];
				break;
			}
			// Overflow
			alphabetIndices[charPos] = 0;   
			passwordGuess[charPos] = alphabet[0];
		}

		// Enlarge password
		if (charPos >= passwordGuess.size())
		{
			passwordGuess.push_back(alphabet[0]);
			alphabetIndices.push_back(0);
		}
	}

	return std::string();
}

std::string dictCrack(uint32* sha1Hash, const std::string& dictPath)
{
	std::ifstream dictFile(dictPath);
	assert(dictFile.is_open() && "Dictionary file could not be opened");

	std::string line;
	while (std::getline(dictFile, line))
	{
		if (CompareToHash(line, sha1Hash))
		{
			dictFile.close();
			return line;
		}
	}

	dictFile.close();
	return std::string("Could not find password in file");
}

int main()
{
	// Define Alphabets
	std::string upperCaseAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string lowerCaseAlphabet = "abcdefghijklmnopqrstuvwxyz";
	std::string numbers = "0123456789";
	std::string specialChars = "!\"#$%&'()*+,-./:;<=>?@[\\]^_{|}~";

	// Given Hashes
	std::string hash1 = "65caa18f6f33d5e89493dc608eb0055126c34997"; // kurz
	std::string hash2 = "d27eb55673c666c0c12873cc6ed592bfe59ff958"; // ?
	std::string hash3 = "fe5f81641092ee46c40e84c4493a757aadc3e65e"; // ?
	std::string hash4 = "de145a203f49e2ef682b434d4e9d241dfe7c4b4f"; // recurvate
	std::string hash5 = "c2ba266015990e8229267fbb1121568b4a5e981d"; // SynWorksDemoAGA
	std::string hash6 = "dd3deb7b9e98f6d189e5c36508b0c55e1697195a"; // overnighter

	// Configure
	// =========================
	auto usedAlphabet = lowerCaseAlphabet;
	auto hash = toHash("word");
	//auto hash = convert160BitHash(hash2);
	// =========================

	// Print Information
	std::cout << "Hash: " << "\t\t\t";
	printWordArrayHex(hash, 5);
	std::cout << "Alphabet (Size: " << usedAlphabet.size() << "): " << "\t" << usedAlphabet << std::endl;

	auto begin = std::chrono::high_resolution_clock::now();

	// Crack Password
	auto bruteforced = bruteForceCrack(hash, usedAlphabet);
	//auto bruteforced = dictCrack(hash, "dic-0294.txt");

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Cracked Password: " << "\t" << bruteforced << std::endl;
	std::cout << "Time needed: " << "\t\t";
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin) << " or ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin) << " or ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin) << " or ";
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - begin) << " or ";
	std::cout << std::chrono::duration_cast<std::chrono::minutes>(end - begin) << std::endl;

	return EXIT_SUCCESS;
}
