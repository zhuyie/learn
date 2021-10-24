// https://preshing.com/20120515/memory-reordering-caught-in-the-act/
// rewrite to c++11 portable version

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include "barrier.h"

// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
class semaphore {
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.

public:
    void release() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void acquire() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while(!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }

    bool try_acquire() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }
};

semaphore beginSema1;
semaphore beginSema2;
semaphore endSema;

int X, Y;
int r1, r2;

void thread1Func(int iterations)
{
    std::mt19937 random;
    random.seed(1);                           // Initialize random number generator
    for (int i = 1; i <= iterations; i++)     // Loop
    {
        beginSema1.acquire();                 // Wait for signal from main thread
        while (random() % 8 != 0) {}          // Add a short, random delay

        // ----- THE TRANSACTION! -----
        X = 1;
        SOME_BARRIER();
        r1 = Y;

        endSema.release();                    // Notify transaction complete
    }
};

void thread2Func(int iterations)
{
    std::mt19937 random;
    random.seed(2);                           // Initialize random number generator
    for (int i = 1; i <= iterations; i++)     // Loop
    {
        beginSema2.acquire();                 // Wait for signal from main thread
        while (random() % 8 != 0) {}          // Add a short, random delay

        // ----- THE TRANSACTION! -----
        Y = 1;
        SOME_BARRIER();
        r2 = X;

        endSema.release();                    // Notify transaction complete
    }
};

int main(int argc, char* argv[])
{
    int iterations = 100000;
    if (argc > 1)
    {
        int n = std::atoi(argv[1]);
        if (n > 0)
            iterations = n;
    }

    printf("BARRIER TYPE = %d\n", BARRIER_TYPE);
    printf("iterations = %d\n", iterations);
    printf("\n");

    std::thread thread1(thread1Func, iterations);
    std::thread thread2(thread2Func, iterations);

    int detected = 0;
    for (int i = 1; i <= iterations; i++)
    {
        // Reset X and Y
        X = 0;
        Y = 0;
        // Signal both threads
        beginSema1.release();
        beginSema2.release();
        // Wait for both threads
        endSema.acquire();
        endSema.acquire();
        // Check if there was a simultaneous reorder
        if (r1 == 0 && r2 == 0)
        {
            detected++;
            printf("%d reorders detected after %d iterations\n", detected, i);
        }
    }

    thread1.join();
    thread2.join();
    return 0;
}
