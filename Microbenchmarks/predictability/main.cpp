#include <benchmark/benchmark.h>

#define SEED (0)

double *array;
double results[3] = {0.0, 0.0, 0.0};

void setup();
void teardown();
double never(const double *array, int n);
double always(const double *array, int n);
double not_so_predictable(const double *array, int n);

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    array = (double *)calloc(n, sizeof(double));

    srand(SEED);

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 10;
    }
}

static void DoTeardown(const benchmark::State &state)
{
    free(array);
}

static void never_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        
        // Do something with the variable so it doesn't get optimized away
        results[0] = never(array, n);
    }
}

static void always_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        
        // Do something with the variable so it doesn't get optimized away
        results[1] = always(array, n);
    }
}

static void not_so_predictable_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);
        
        // Do something with the variable so it doesn't get optimized away
        results[2] = not_so_predictable(array, n);
    }
}

double never(const double *array, int n)
{
    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] > 20.0)
        {
            result = array[i];
        }
    }

    return result;
}

double always(const double *array, int n)
{
    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] >= 0.0 && array[i] < 10.0)
        {
            result = array[i];
        }
    }

    return result;
}

double not_so_predictable(const double *array, int n)
{
    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] > 5.0)
        {
            result = array[i];
        }
    }

    return result;
}

#define N (1 << 20)

BENCHMARK(never_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(always_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);
BENCHMARK(not_so_predictable_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(N);

BENCHMARK_MAIN();