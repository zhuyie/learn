#include <stdio.h>
#include <string.h>
#include <fenv.h>

const int LOOP_COUNT = 100000000;

int test1()
{
	float x = 1.0f;
	for (int i = 0; i < LOOP_COUNT; i++)
	{
		x /= 1.1f;
		x += 0.1f;
		x -= 0.1f;
	}
	printf("x = %f\n", x);
	return 0;
}

int test2()
{
	float x = 1.0f;
	for (int i = 0; i < LOOP_COUNT; i++)
	{
		x /= 1.1f;
		x += 0;
		x -= 0;
	}
	printf("x = %f\n", x);
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc >= 2 && strcmp(argv[1], "2") == 0)
	{
		test2();
	}
	else if (argc >= 2 && strcmp(argv[1], "3") == 0)
	{
		fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV);
		test2();
	}
	else
	{
		test1();
	}
}
