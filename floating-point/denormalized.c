#include <stdio.h>
#include <string.h>

int test1()
{
	float x = 1.1;
	float z = 1.123;
	float y = x;
	for (int j = 0; j < 90000000; j++)
	{
		y *= x;
		y /= z;
		y += 0.1f;
		y -= 0.1f;
	}
	return 0;
}

int test2()
{
	float x = 1.1;
	float z = 1.123;
	float y = x;
	for (int j = 0; j < 90000000; j++)
	{
		y *= x;
		y /= z;
		y += 0;
		y -= 0;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc >= 2 && strcmp(argv[1], "2") == 0)
	{
		test2();
	}
	else
	{
		test1();
	}
}
