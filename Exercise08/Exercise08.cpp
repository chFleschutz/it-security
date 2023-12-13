#include <iostream>
#include <vector>

int primaryRepresentant(int x, int m)
{
	auto xMod = x % m;
	return xMod < 0 ? xMod + m : xMod;
}

bool isEquivalent(int x, int y, int m)
{
	return primaryRepresentant(x, m) == primaryRepresentant(y, m);
}

int main()
{
	struct Test
	{
		int x;
		int y;
		int m;
		bool expected;
	};

	std::vector<Test> test_cases;
	test_cases.push_back({ 1, 2, 3, false });
	test_cases.push_back({ 1, 4, 3, true });
	test_cases.push_back({ 1, 5, 3, false });
	test_cases.push_back({ 10, 17, 7, true });
	test_cases.push_back({ 33, 7, 4, false });
	test_cases.push_back({ -7, 8, 5, true });
	test_cases.push_back({ -7, 10, 5, false });
	test_cases.push_back({ -8, 12, 5, true });
	test_cases.push_back({ -7, 17, 5, false });
	test_cases.push_back({ -7, -22, 3, true });

	auto colorGreen = "\x1B[32m";
	auto colorRed = "\x1B[31m";
	auto defaultColor = "\033[0m";

	std::cout << "Test cases for isEquivalent(x, y, m):" << std::endl;
	std::cout << "x\t y\t m\t -> result \texpected" << std::boolalpha << std::endl;
	std::cout << "------------------------------------------------" << std::endl;

	int casesPassed = 0;
	for (const auto& test : test_cases)
	{
		auto result = isEquivalent(test.x, test.y, test.m);
		auto color = colorRed;
		if (result == test.expected)
		{
			casesPassed++;
			color = defaultColor;
		}

		std::cout << color << test.x << "\t " << test.y << "\t " << test.m << "\t -> " << result << " \t" << test.expected << defaultColor << std::endl;
	}

	auto color = casesPassed == test_cases.size() ? colorGreen : colorRed;
	std::cout << std::endl << color << "Passed " << casesPassed << " of " << test_cases.size() << " test cases." << defaultColor << std::endl;
}
