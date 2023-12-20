#include <iostream>
#include <vector>
#include <chrono>


bool isPrime(int x)
{
	if (x < 2)
		return false;

	int rootX = sqrt(x);
	for (int i = 2; i <= rootX; i++)
	{
		if (x % i == 0)
			return false;
	}

	return true;
}

int main()
{
	struct Test
	{
		int x;
		bool expected;
	};

	// A prime number is a natural number greater than 1 that is not a product of two smaller natural numbers
	std::vector<Test> test_cases;
	test_cases.push_back({ -1, false });
	test_cases.push_back({ 0, false });
	test_cases.push_back({ 1, false });
	test_cases.push_back({ 2, true });
	test_cases.push_back({ 3, true });
	test_cases.push_back({ 4, false });
	test_cases.push_back({ 5, true });
	test_cases.push_back({ 6, false });
	test_cases.push_back({ 7, true });
	test_cases.push_back({ 8, false });
	test_cases.push_back({ 9, false });
	test_cases.push_back({ 10, false });
	test_cases.push_back({ 11, true });
	test_cases.push_back({ 12, false });
	test_cases.push_back({ 13, true });
	test_cases.push_back({ 14, false });
	test_cases.push_back({ 15, false });
	test_cases.push_back({ 16, false });
	test_cases.push_back({ 17, true });
	test_cases.push_back({ 18, false });
	test_cases.push_back({ 19, true });
	test_cases.push_back({ 20, false });
	test_cases.push_back({ 21, false });
	test_cases.push_back({ 22, false });
	test_cases.push_back({ 23, true });
	test_cases.push_back({ 24, false });
	test_cases.push_back({ 25, false });
	test_cases.push_back({ 26, false });
	test_cases.push_back({ 27, false });
	test_cases.push_back({ 28, false });
	test_cases.push_back({ 29, true });
	test_cases.push_back({ 30, false });
	test_cases.push_back({ 181, true });
	test_cases.push_back({ 191, true });
	test_cases.push_back({ 193, true });
	test_cases.push_back({ 197, true });
	test_cases.push_back({ 199, true });
	test_cases.push_back({ 211,true });
	test_cases.push_back({ 1000000, false });
	test_cases.push_back({ 1000003, true });
	test_cases.push_back({ 1000007, false });
	test_cases.push_back({ 1000009, false });
	test_cases.push_back({ 1000013, false });
	test_cases.push_back({ 1000014, false });
	test_cases.push_back({ 69696969, false });
	test_cases.push_back({ 2147483643, false });
	test_cases.push_back({ 2147483644, false });
	test_cases.push_back({ 2147483645, false });
	test_cases.push_back({ 2147483646, false });
	test_cases.push_back({ 2147483647, true });

	auto colorGreen = "\x1B[32m";
	auto colorRed = "\x1B[31m";
	auto defaultColor = "\033[0m";

	std::cout << "Test cases for isPrime(x):" << std::endl;
	std::cout << "x\t -> result \texpected" << std::boolalpha << std::endl;
	std::cout << "------------------------------------------------" << std::endl;

	int casesPassed = 0;
	auto begin = std::chrono::high_resolution_clock::now();
	for (const auto& test : test_cases)
	{
		auto result = isPrime(test.x);

		//casesPassed += result == test.expected; // uncomment this and comment the rest in this loop to disable console output
		auto color = colorRed;
		if (result == test.expected)
		{
			casesPassed++;
			color = defaultColor;
		}

		std::cout << color << test.x << "\t -> " << result << " \t" << test.expected << defaultColor << std::endl;
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	auto color = casesPassed == test_cases.size() ? colorGreen : colorRed;
	std::cout << color;
	std::cout << "\nPassed " << casesPassed << " of " << test_cases.size() << " test cases (Time: " << duration << ") " << std::endl;
	std::cout << defaultColor;
}
