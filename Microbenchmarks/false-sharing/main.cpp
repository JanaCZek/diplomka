#include <stdint.h>
#include <mutex>
#include <omp.h>
#include <benchmark/benchmark.h>

int32_t shared;
std::mutex mutex;

volatile int32_t dataA, dataB, dataC, dataD;

#define ALIGNMENT (64)

alignas(ALIGNMENT) volatile int32_t dataAlignedA;
alignas(ALIGNMENT) volatile int32_t dataAlignedB;
alignas(ALIGNMENT) volatile int32_t dataAlignedC;
alignas(ALIGNMENT) volatile int32_t dataAlignedD;

void true_sharing(int n);
void false_sharing(int n);
void false_sharing_fix(int n);

static void DoSetup(const benchmark::State &state)
{
    shared = 0;

    dataA = 0;
    dataB = 0;
    dataC = 0;
    dataD = 0;

    dataAlignedA = 0;
    dataAlignedB = 0;
    dataAlignedC = 0;
    dataAlignedD = 0;
}

static void DoTeardown(const benchmark::State &state)
{
}

static void true_sharing_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        true_sharing(n);
    }
}

static void false_sharing_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        false_sharing(n);
    }
}

static void false_sharing_fix_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        false_sharing_fix(n);
    }
}

void true_sharing(int n)
{
#pragma omp parallel num_threads(4)
    for (int i = 0; i < n; i++)
    {
        mutex.lock();
        ++shared;
        mutex.unlock();
    }
}
void false_sharing(int n)
{
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        switch (thread_num)
        {
        case 0:
            for (int i = 0; i < n; i++)
            {
                ++dataA;
            }
            break;
        case 1:
            for (int i = 0; i < n; i++)
            {
                ++dataB;
            }
            break;
        case 2:
            for (int i = 0; i < n; i++)
            {
                ++dataC;
            }
            break;
        case 3:
            for (int i = 0; i < n; i++)
            {
                ++dataD;
            }
            break;
        }
    }
}
void false_sharing_fix(int n)
{
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        switch (thread_num)
        {
        case 0:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedA;
            }
            break;
        case 1:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedB;
            }
            break;
        case 2:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedC;
            }
            break;
        case 3:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedD;
            }
            break;
        }
    }
}

#define N (1 << 20)

BENCHMARK(true_sharing_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(false_sharing_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(false_sharing_fix_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);

BENCHMARK_MAIN();