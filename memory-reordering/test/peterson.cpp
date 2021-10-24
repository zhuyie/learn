#include <cstdio>
#include <thread>
#include "barrier.h"

// Simple class implementing Peterson's algorithm for mutual exclusion
class Peterson {
private:
    // Is this thread interested in the critical section
    volatile int interested[2] = {0, 0};

    // Who's turn is it?
    volatile int turn = 0;

public:
    void lock(int tid) {
        // Mark that this thread wants to enter the critical section
        interested[tid] = 1;

        // Assume the other thread has priority
        int other = 1 - tid;
        turn = other;

        SOME_BARRIER();

        // Wait until the other thread finishes or is not interested
        while (turn == other && interested[other])
            ;
    }
    void unlock(int tid) {
        // Mark that this thread is no longer interested
        interested[tid] = 0;
    }
};

// Work function
void work(Peterson &p, int &val, int tid) {
    for (int i = 0; i < 1e8; i++) {
        // Lock using Peterson's algorithm
        p.lock(tid);
        // Critical section
        val++;
        // Unlock using Peterson's algorithm
        p.unlock(tid);
    }
}

int main()
{
    printf("BARRIER TYPE = %d\n", BARRIER_TYPE);
    
    // Shared value
    int val = 0;
    Peterson p;

    // Create threads
    std::thread t0([&] { work(p, val, 0); });
    std::thread t1([&] { work(p, val, 1); });

    // Wait for the threads to finish
    t0.join();
    t1.join();

    // Print the result
    printf("FINAL VALUE IS %d\n", val);

    return 0;
}
