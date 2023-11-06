#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <limits>

int countChars(const std::string& path, std::map<char, int>& charCount)
{
	std::ifstream file(path);
	assert(file.is_open() && "File could not be opened");

	int count = 0;
	std::string line;
	while (std::getline(file, line))
	{
		for (const auto& c : line)
		{
			if (c == ' ' or c == '\t')
				continue;

			count++;
			charCount[c]++;
		}
	}

	return count;
}

void printResults(const std::map<char, int>& charCount, int maxChars)
{
	int width = 16;
	std::cout << std::setprecision(3) << std::fixed
		<< std::setw(width) << "Char" 
		<< std::setw(width) << "Count" 
		<< std::setw(width) << "Probability %" 
		<< std::setw(width) << "Entropy" 
		<< std::endl;

	float shannon = 0.0f;
	float minEntropy = std::numeric_limits<float>::max();

	for (const auto& [c, count] : charCount)
	{
		float probability = static_cast<float>(count) / maxChars;
		float entropy = log2(1.0f / probability);
		
		shannon += probability * entropy;
		minEntropy = std::min(minEntropy, entropy);

		std::cout
			<< std::setw(width) << c
			<< std::setw(width) << count
			<< std::setw(width) << probability * 100.0f // percent
			<< std::setw(width) << entropy 
			<< std::endl;
	}

	std::cout << std::endl;
	std::cout << "Shannon entropy:\t" << shannon << std::endl;
	std::cout << "Min entropy:    \t" << minEntropy << std::endl; 
}

int main()
{
	std::string filePath = "text.txt";

	std::map<char, int> charCount;
	int maxChars = countChars(filePath, charCount);

	printResults(charCount, maxChars);

	return EXIT_SUCCESS;
}