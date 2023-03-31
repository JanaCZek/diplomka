#include <benchmark/benchmark.h>

#define ACCESS_COUNT (10000)

double *array;
double result;

double cache_associativity_limit(int step, int n);

static void DoSetup(const benchmark::State &state)
{
    int step = state.range(0);
    int n = ACCESS_COUNT * step;

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

static void cache_associativity_limit_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = cache_associativity_limit(step, n);

        // Do something with the variable so it doesn't get optimized away
        result = sum;
    }
}

double cache_associativity_limit(int step, int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i += step)
    {
        sum += array[i];
    }

    return sum;
}

BENCHMARK(cache_associativity_limit_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(30)->Arg(32)->Arg(60)->Arg(64)->Arg(120)->Arg(128)->Arg(250)->Arg(256)->Arg(510)->Arg(512);

BENCHMARK_MAIN();