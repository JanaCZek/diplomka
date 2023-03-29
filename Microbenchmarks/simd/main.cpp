#include <intrin.h>
#include <benchmark/benchmark.h>

double *array;
double results[2] = {0.0, 0.0};



double array_sum(int n);
double array_sum_simd(int n);

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    array = (double *)_aligned_malloc(sizeof(double) * n, 16);

    for (int i = 0; i < n; i++)
    {
        array[i] = (i + 1) % 100;
    }
}

static void DoTeardown(const benchmark::State &state)
{
    // free(array);
}

static void array_sum_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = array_sum(n);

        // Do something with the variable so it doesn't get optimized away
        results[0] = sum;
    }
}

static void array_sum_simd_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = array_sum_simd(n);

        // Do something with the variable so it doesn't get optimized away
        results[1] = sum;
    }
}

double array_sum(int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }

    return sum;
}

double array_sum_simd(int n)
{
    // SSE specific step
    int step = 128 / (sizeof(double) * 8);
    int sumSize = step;
    double *sum = (double *)malloc(sizeof(double) * sumSize);

    __m128d sumVector = _mm_loadu_pd(sum);
    __m128d *arrayVector = (__m128d *)array;
    int vecIterations = n / step;

    for (int i = 0; i < vecIterations; i++)
    {
        sumVector = _mm_add_pd(arrayVector[i], sumVector);
    }

    _mm_store_pd(sum, sumVector);

    double sumTotal = 0.0;

    for (int i = 0; i < sumSize; i++)
    {
        sumTotal += sum[i];
    }

    return sumTotal;
}

#define START (1 << 10)
#define END (1 << 20)

BENCHMARK(array_sum_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(array_sum_simd_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);

BENCHMARK_MAIN();