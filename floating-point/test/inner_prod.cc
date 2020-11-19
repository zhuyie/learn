#include <cstdio>
#include <xmmintrin.h>
#include <chrono>   
using namespace std::chrono;

const int N = 784;

void test_Simple(float *input, float *weight, float bias, int benchN)
{
    float res = 0;
    auto start = system_clock::now();
    for (int b = 0; b < benchN; b++)
    {
        res = 0;
        for (int i = 0; i < N; i++)
        {
            res += input[i] * weight[i];
        }
        res += bias;
    }
    auto end = system_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    printf("Simple: %f %lldms\n", res, duration.count());
}

inline float __pfirst(const __m128& a) { float x = _mm_cvtss_f32(a); return x; }

inline float __predux(const __m128& a)
{
    __m128 tmp = _mm_add_ps(a, _mm_movehl_ps(a, a));
    return __pfirst(_mm_add_ss(tmp, _mm_shuffle_ps(tmp, tmp, 1)));
}

void test_SSE(float *input, float *weight, float bias, int benchN)
{
    float res;
    auto start = system_clock::now();
    for (int b = 0; b < benchN; b++)
    {
        __m128 c = {0};
        for (int i = 0; i < N; i += 4)
        {
            __m128 x = _mm_load_ps(input + i);
            __m128 y = _mm_load_ps(weight + i);
            c = _mm_add_ps(c, _mm_mul_ps(x, y));
        }
        res = __predux(c);
        res += bias;
    }
    auto end = system_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    printf("SSE   : %f %lldms\n", res, duration.count());
}

void test_SimSSE(float *input, float *weight, float bias, int benchN)
{
    float tmp0, tmp1, res, tmp2, tmp3;
    auto start = system_clock::now();
    for (int b = 0; b < benchN; b++)
    {
        tmp0 = tmp1 = tmp2 = tmp3 = 0;
        for (int i = 0; i < N; i += 4)
        {
            tmp0 += input[i+0] * weight[i+0];
            tmp1 += input[i+1] * weight[i+1];
            tmp2 += input[i+2] * weight[i+2];
            tmp3 += input[i+3] * weight[i+3];
        }
        res = (tmp0 + tmp2) + (tmp1 + tmp3);
        res += bias;
    }
    auto end = system_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    printf("SimSSE: %f %lldms\n", res, duration.count());
}

int main()
{
    float input[N] __attribute__ ((aligned(16)));
    float weight[N] __attribute__ ((aligned(16)));
    float bias;
    FILE *fp = fopen("inner_prod_data", "rb");
    if (!fp)
    {
        printf("Open file failed\n");
        return 1;
    }
    if (fread(input, sizeof(float), N, fp) != N ||
        fread(weight, sizeof(float), N, fp) != N ||
        fread(&bias, sizeof(float), 1, fp) != 1)
    {
        fclose(fp);
        printf("Read file failed\n");
        return 1;
    }
    fclose(fp);

    int benchN = 5000000;
    test_Simple(input, weight, bias, benchN);
    test_SSE(input, weight, bias, benchN);
    test_SimSSE(input, weight, bias, benchN);

    return 0;
}
