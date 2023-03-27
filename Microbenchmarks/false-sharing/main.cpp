#include <stdint.h>
#include <mutex>
#include <omp.h>
#include <thread>
#include <iostream>
#include <benchmark/benchmark.h>

std::atomic<int32_t> shared;
std::mutex mutex;

// int32_t results[4] = {0};

struct Data
{
    int32_t a, b, c, d;
};

#define ALIGNMENT (64)

struct DataAligned
{
    alignas(ALIGNMENT) int32_t a;
    alignas(ALIGNMENT) int32_t b;
    alignas(ALIGNMENT) int32_t c;
    alignas(ALIGNMENT) int32_t d;
};

void setup();
void teardown();
int32_t true_sharing(int n);
int32_t false_sharing(int n);
int32_t false_sharing_fix(int n);

static void DoSetup(const benchmark::State &state)
{
    shared = 0;

    // data.a = 0;
    // data.b = 0;
    // data.c = 0;
    // data.d = 0;

    // dataAligned.a = 0;
    // dataAligned.b = 0;
    // dataAligned.c = 0;
    // dataAligned.d = 0;
}

static void DoTeardown(const benchmark::State &state)
{
}

static void true_sharing_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        int32_t result = true_sharing(n);

        // Do something with the variable so it doesn't get optimized away
        // results[0] = data[0].d;
    }
}

static void false_sharing_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        int32_t result = false_sharing(n);

        // Do something with the variable so it doesn't get optimized away
        // results[1] = dataOrdered[1].a;
    }
}

static void false_sharing_fix_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        int32_t result = false_sharing_fix(n);

        // Do something with the variable so it doesn't get optimized away
        // results[1] = dataOrdered[1].a;
    }
}

int32_t true_sharing(int n)
{
#pragma omp parallel num_threads(4)
    for (int i = 0; i < n; i++)
    {
        // mutex.lock();
        ++shared;
        // mutex.unlock();
    }

    return shared;
}
int32_t false_sharing(int n)
{
    Data data;
    data.a = 0;
    data.b = 0;
    data.c = 0;
    data.d = 0;
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        for (int i = 0; i < n; i++)
        {
            switch (thread_num)
            {
            case 0:
                ++data.a;
            case 1:
                ++data.b;
            case 2:
                ++data.c;
            case 3:
                ++data.d;
            }
        }
    }
    return data.a;
}
int32_t false_sharing_fix(int n)
{
    DataAligned dataAligned;
    dataAligned.a = 0;
    dataAligned.b = 0;
    dataAligned.c = 0;
    dataAligned.d = 0;
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        for (int i = 0; i < n; i++)
        {
            switch (thread_num)
            {
            case 0:
                ++dataAligned.a;
            case 1:
                ++dataAligned.b;
            case 2:
                ++dataAligned.c;
            case 3:
                ++dataAligned.d;
            }
        }
    }
    return dataAligned.a;
}

#define N (1 << 14)
// int main()
// {
//     Data data;
//     data.a = 0;
//     data.b = 0;
//     data.c = 0;
//     data.d = 0;

//     DataAligned dataAligned;
//     dataAligned.a = 0;
//     dataAligned.b = 0;
//     dataAligned.c = 0;
//     dataAligned.d = 0;

//     printf("%p\t", &data.a);
//     printf("%p\t", &data.b);
//     printf("%p\t", &data.c);
//     printf("%p\t", &data.d);
//     printf("\n");

//     printf("%p\t", &dataAligned.a);
//     printf("%p\t", &dataAligned.b);
//     printf("%p\t", &dataAligned.c);
//     printf("%p\t", &dataAligned.d);
//     printf("\n");

//     auto start = std::chrono::steady_clock::now();
//     int32_t result = true_sharing(N);
//     auto end = std::chrono::steady_clock::now();
//     std::chrono::duration<double> elapsed_seconds = end - start;
//     std::cout << result << std::endl;
//     std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

//     start = std::chrono::steady_clock::now();
//     result = false_sharing(N);
//     end = std::chrono::steady_clock::now();
//     elapsed_seconds = end - start;
//     std::cout << result << std::endl;
//     std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

//     start = std::chrono::steady_clock::now();
//     result = false_sharing_fix(N);
//     end = std::chrono::steady_clock::now();
//     elapsed_seconds = end - start;
//     std::cout << result << std::endl;
//     std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
// }

BENCHMARK(true_sharing_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(false_sharing_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(false_sharing_fix_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);

BENCHMARK_MAIN();