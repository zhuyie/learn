#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <chrono>
using namespace std::chrono;

#define n 2048

double A[n][n];
double B[n][n];
double C[n][n];

int main(int argc, char* argv[])
{
    // parsing command line
    bool fastMode = false;
    if (argc > 1 && strcmp(argv[1], "1") == 0) {
        fastMode = true;
    }

    // populate the matrices with random values between 0.0 and 1.0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)rand() / (double)RAND_MAX;
            B[i][j] = (double)rand() / (double)RAND_MAX;
            C[i][j] = 0;
        }
    }

    // matrix multiplication
    auto start = system_clock::now();
    if (!fastMode) {
        // i -> j -> k
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    } else {
        // i -> k -> j
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                for (int j = 0; j < n; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    auto elapsedTime = duration_cast<microseconds>(system_clock::now() - start);

    fprintf(stdout, "fast = %d\n", fastMode);
    fprintf(stdout, "time = %.2fms\n", elapsedTime.count()/1000.0);

    return 0;
}
