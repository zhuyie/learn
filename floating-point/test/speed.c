#include <stdio.h>
#include <string.h>

volatile int   g_int;
volatile float g_float;

void test1()
{
    for (int i = 0; i < 50000; i++)
    {
        g_int = 0;
        for (int j = 0; j < 50000; j++)
        {
            g_int += 6;
            g_int -= 5;
        }
    }
}

void test2()
{
    for (int i = 0; i < 50000; i++)
    {
        g_float = 0;
        for (int j = 0; j < 50000; j++)
        {
            g_float += 6.0f;
            g_float -= 5.0f;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "float") == 0)
    {
        test2();
        printf("%f\n", g_float);
    }
    else
    {
        test1();
        printf("%d\n", g_int);
    }
    return 0;
}
