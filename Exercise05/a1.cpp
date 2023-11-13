#include <stdio.h>
#include <iostream>

int g(int x) 
{
	static bool b = false;
	if (b)
		return 0;

	b = true;

	int* ptr = &x;
	while (*ptr != 7)
		ptr++;

	*(ptr + 8) = -2000000000; // -2 Mrd

	return 0;
}

void f() 
{
	int y = 7;
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("i: %d, g(i): %d\n", i, g(i));
	}
	printf("value of y: %d\n", y);
}

int main()
{
	f();
	return 0;
}