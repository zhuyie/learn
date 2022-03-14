#include <cstdio>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std::chrono;

static int dummy[2];

static int64_t matchlen(const char *oldbuf, int64_t oldsize, const char *newbuf, int64_t newsize)
{
    int64_t i = 0;
    int64_t count = (oldsize < newsize) ? oldsize : newsize;
	for (; i < count; i++) {
		if (oldbuf[i] != newbuf[i])
			break;
	}
	return i;
}

int main(int argc, char* argv[])
{
    int64_t n = matchlen(argv[0], strlen(argv[0]), "world", 5);
    if (n == 65535) {
        return -1;
    }

    // parsing command line
    bool sort = false;
    if (argc > 1 && strcmp(argv[1], "1") == 0) {
        sort = true;
    }

    // prepare test data (uniformly distributed random values)
    const int arraySize = 5000;
    std::vector<int> data;
    data.resize(arraySize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 256);
    for (int i = 0; i < arraySize; i++) {
        data[i] = dis(gen);
    }

    // sort or not
    if (sort) {
        std::sort(data.begin(), data.end());
    }

    auto start = system_clock::now();
    unsigned int count[2];
    const int testN = 100000;
    for (int t = 0; t < testN; ++t) {
        // main loop
        memset(count, 0, sizeof(count));
        for (int i = 0; i < arraySize; ++i) {
            if (data[i] < 128) {
                dummy[0] *= dummy[1];  // consume-a-few-cycles
                count[0]++;            // counting
            } else {
                dummy[1] *= dummy[0];  // consume-a-few-cycles
                count[1]++;            // counting
            }
        }
    }
    auto duration = duration_cast<microseconds>(system_clock::now() - start);
    
    fprintf(stdout, "sort  = %d\n", sort ? true : false);
    fprintf(stdout, "total = %d\n", arraySize);
    fprintf(stdout, "count = [%u,%u]\n", count[0], count[1]);
    fprintf(stdout, "duration = %.2fms\n", duration.count()/1000.0);

    return 0;
}
