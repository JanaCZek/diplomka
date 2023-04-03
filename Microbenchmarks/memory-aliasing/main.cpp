#include <benchmark/benchmark.h>

#define N (1 << 20)

double *array;

void write_read(double *src, double *dest, int n);

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

static void write_read_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        write_read(&array[0], &array[1], n);
    }
}

static void write_read_aliased_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        write_read(&array[0], &array[0], n);
    }
}

void write_read(double *src, double *dest, int n)
{
    int count = n;
    int val = 0;

    while (count)
    {
        *dest = val;
        val = (*src) + 1;
        count--;
    }
}

BENCHMARK(write_read_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(N);
BENCHMARK(write_read_aliased_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(N);

BENCHMARK_MAIN();