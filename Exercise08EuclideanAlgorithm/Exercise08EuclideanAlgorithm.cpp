#include <iostream>
#include <vector>

int32_t gcd(int32_t a, int32_t b)
{
	if (b == 0)
		return a;

	return gcd(b, a % b);
}

int main()
{
	struct Test
	{
		int32_t a;
		int32_t b;
		int32_t expected;
	};

	std::vector<Test> testCases;
	testCases.push_back({ 0, 0, 0 });
	testCases.push_back({ 1, 2, 1 });
	testCases.push_back({ 2, 4, 2 });
	testCases.push_back({ 4, 2, 2 });
	testCases.push_back({ 2, 3, 1 });
	testCases.push_back({ 3, 2, 1 });
	testCases.push_back({ 3, 6, 3 });
	testCases.push_back({ 6, 3, 3 });
	testCases.push_back({ 6, 9, 3 });
	testCases.push_back({ 9, 6, 3 });
	testCases.push_back({ 9, 12, 3 });
	testCases.push_back({ 10, 5, 5 });
	testCases.push_back({ 15, 10, 5 });
	testCases.push_back({ 16, 12, 4 });
	testCases.push_back({ 12, 16, 4 });
	testCases.push_back({ 12, 18, 6 });
	testCases.push_back({ 19, 12, 1 });
	testCases.push_back({ 42, 56, 14 });
	testCases.push_back({ 56, 42, 14 });
	testCases.push_back({ 42, 0, 42 });
	testCases.push_back({ 0, 42, 42 });
	testCases.push_back({ 42, 42, 42 });
	testCases.push_back({ 461952, 116298, 18 });
	testCases.push_back({ 7966496, 314080416, 32 });
	testCases.push_back({ 24826148, 45296490, 526 });

	auto colorGreen = "\x1B[32m";
	auto colorRed = "\x1B[31m";
	auto defaultColor = "\033[0m";

	std::cout << "Test cases for gcd(a, b):" << std::endl;
	std::cout << "a\t b\t -> result \texpected" << std::boolalpha << std::endl;
	std::cout << "------------------------------------------------" << std::endl;

	int casesPassed = 0;
	for (const auto& test : testCases)
	{
		auto result = gcd(test.a, test.b);
		auto color = colorRed;
		if (result == test.expected)
		{
			casesPassed++;
			color = defaultColor;
		}

		std::cout << color << test.a << "\t " << test.b << "\t -> " << result << " \t" << test.expected << defaultColor << std::endl;
	}

	auto color = casesPassed == testCases.size() ? colorGreen : colorRed;
	std::cout << std::endl << color << "Passed " << casesPassed << " of " << testCases.size() << " test cases." << defaultColor << std::endl;
}
