#include <stdio.h>

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

int main()
{
	test1();
	test2();
	test3();
	return 0;
}
