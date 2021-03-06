#include <stdio.h>
#include <limits.h> // for INT_MAX
#include <float.h>  // for FLT_MAX

void test1()
{
	printf("test1:\n");
	// https://www.jianshu.com/p/fad987f1642d
	double a = 2222222;
	double b = 269567.53;
	double result = a + b;
	printf("------------------------------\n");
	printf("a = %.10f\n", a);
	printf("b = %.10f\n", b);
	printf("------------------------------\n");
	printf("a + b = %.10f\n", result);
	printf("\n");
}

void test2()
{
	printf("test2:\n");
	// floating-point addition and multiplication are not necessarily associative.
	float a = 1234.567f;
	float b = 45.67834f;
	float c = 1.000308f;
	float result0 = (a + b) + c;
	float result1 = a + (b + c);
	printf("------------------------------\n");
	printf("a = %f\n", a);
	printf("b = %f\n", b);
	printf("c = %f\n", c);
	printf("------------------------------\n");
	printf("(a + b) + c = %f\n", result0);
	printf("a + (b + c) = %f\n", result1);
	printf("\n");
}

void test3()
{
	printf("test3:\n");
	// floating-point addition and multiplication are not necessarily distributive.
	float a = 1234.567f;
	float b = 1.234567f;
	float c = 3.000001f;
	float result0 = (a + b) * c;
	float result1 = a * c + b * c;
	printf("------------------------------\n");
	printf("a = %f\n", a);
	printf("b = %f\n", b);
	printf("c = %f\n", c);
	printf("------------------------------\n");
	printf("(a + b) * c   = %f\n", result0);
	printf("a * c + b * c = %f\n", result1);
	printf("\n");
}

void test4()
{
	printf("test4:\n");
	printf("------------------------------\n");

	float f0 = 567.8f;
	printf("float(%f) => int(%d)\n", f0, (int)f0);
	
	for (int i = 0; i < 8; i++)
	{
		int n = 16777213 + i;
		printf("int(%d) => float(%f)\n", n, (float)n);
	}

	int i1 = 1000000000;
	printf("int(%d) => float(%f)\n", i1, (float)i1);
	int i2 = 1000000101;
	printf("int(%d) => float(%f)\n", i2, (float)i2);

	int i3 = INT_MAX;
	printf("int(%d) => float(%f)\n", i3, (float)i3);
	float f4 = INT_MAX;  f4 += 1.0f;
	printf("float(%f) => int(%d 0x%08x)\n", f4, (int)f4, (int)f4);
	long long i5 = (long long)INT_MAX + 1;
	printf("long long(%lld) => int(%d)\n", i5, (int)i5);
	
	float f6 = FLT_MAX;
	printf("float(%f) => int(%d)\n", f6, (int)f6);

	printf("\n");
}

int main()
{
	test1();
	test2();
	test3();
	test4();
	return 0;
}
