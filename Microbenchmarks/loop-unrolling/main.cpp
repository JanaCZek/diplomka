#include <benchmark/benchmark.h>

double *array;
double results[3] = { 0.0, 0.0, 0.0 };

void array_sum(double *array, int n, double *result);
void array_sum_unrolled_2(double *array, int n, double *result);
void array_sum_unrolled_4(double *array, int n, double *result);

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    array = (double *)calloc(n, sizeof(double));

    for (int i = 0; i < n; i++)
    {
        array[i] = (i + 1) % 100;
    }
}

static void DoTeardown(const benchmark::State &state)
{
    free(array);
}

static void array_sum_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        double sum = 0.0;

        array_sum(array, n, &sum);

        // Do something with the variable so it doesn't get optimized away
        results[0] = sum;
    }
}

static void array_sum_unrolled_2_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        double sum = 0.0;

        array_sum_unrolled_2(array, n, &sum);
        
        // Do something with the variable so it doesn't get optimized away
        results[1] = sum;
    }
}

static void array_sum_unrolled_4_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        double sum = 0.0;

        array_sum_unrolled_4(array, n, &sum);
        
        // Do something with the variable so it doesn't get optimized away
        results[2] = sum;
    }
}

void array_sum(double *array, int n, double *result)
{
    for (int i = 0; i < n; i++)
    {
        *result += array[i];
    }
}
void array_sum_unrolled_2(double *array, int n, double *result)
{
    int i = 0;
    int limit = n - 1;
    double accumulator = 0.0;

    // Unrolled loop
    for (; i < limit; i += 2)
    {
        accumulator += array[i] + array[i + 1];
    }

    // Finalizing loop
    for (; i < n; i++)
    {
        accumulator += array[i];
    }

    *result = accumulator;
}

void array_sum_unrolled_4(double *array, int n, double *result)
{
    int i = 0;
    int limit = n - 3;
    double accumulator = 0.0;

    // Unrolled loop
    for (; i < limit; i += 4)
    {
        accumulator += array[i] + array[i + 1] + array[i + 2] + array[i + 3];
    }

    // Finalizing loop
    for (; i < n; i++)
    {
        accumulator += array[i];
    }

    *result = accumulator;
}

#define START (1 << 4)
#define END (1 << 15)

BENCHMARK(array_sum_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(array_sum_unrolled_2_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(array_sum_unrolled_4_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);

BENCHMARK_MAIN();