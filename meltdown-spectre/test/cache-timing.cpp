#if !(defined(__x86_64__) || defined(__i386__))
# error "Only x86-64 and i386 are supported at the moment"
#endif

#include <cstdio>
#include <cstdlib>
#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

static inline int
get_access_time(volatile char *addr)
{
    unsigned long long time1, time2;
    unsigned junk;
    time1 = __rdtscp(&junk);
    (void)*addr;
    time2 = __rdtscp(&junk);
    return (int)(time2 - time1);
}

#define LOOP_TIMES 1000000

int main(int argc, char* argv[])
{
    char *array = (char*)malloc(1000);
    array[0] = 'x';

    int cached = 0, uncached = 0;
    for (int i = 0; i < LOOP_TIMES; i++) {
        cached += get_access_time(array);
    }
    for (int i = 0; i < LOOP_TIMES; i++) {
        _mm_clflush(array);
        uncached += get_access_time(array);
    }
    cached /= LOOP_TIMES;
    uncached /= LOOP_TIMES;
    fprintf(stdout, "cached = %d, uncached = %d\n", cached, uncached);

    free(array);
    return 0;
}
