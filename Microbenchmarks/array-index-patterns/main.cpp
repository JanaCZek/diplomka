#include <benchmark/benchmark.h>

#define ACCESS_COUNT (10000)

double *array;
double results[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

void setup();
void teardown();
double sequential(int n);
double strided(int step, int n);
double random(int n);
double pattern(int n);
double pattern_prefetching(int n);

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

static void sequential_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = sequential(n);

        results[0] = sum;
    }
}

static void strided_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = strided(step, n);

        results[1] = sum;
    }
}

static void random_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = random(n);

        results[2] = sum;
    }
}

static void pattern_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = pattern(n);

        results[3] = sum;
    }
}

static void pattern_prefetching_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT * step;

        double sum = pattern_prefetching(n);

        results[4] = sum;
    }
}

double sequential(int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }

    return sum;
}

double strided(int step, int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i += step)
    {
        sum += array[i];
    }

    return sum;
}

double random(int n)
{
    double sum = 0.0;
    unsigned int seed = 42;
    srand(seed);

    for (int i = 0; i < n; i++)
    {
        int index = rand() % n;
        sum += array[index];
    }

    return sum;
}

double pattern(int n) {

    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        int index = (2 * i + 5) % n;
        sum += array[index];
    }

    return sum;
}

double pattern_prefetching(int n) {
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        __builtin_prefetch(&array[(2 * i + 5) % n]);
        int index = (2 * i + 5) % n;
        sum += array[index];
    }

    return sum;
}

// BENCHMARK(random_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(1);
// BENCHMARK(sequential_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(1);
BENCHMARK(pattern_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(1);
BENCHMARK(pattern_prefetching_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Arg(1);
// BENCHMARK(strided_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(1 << 1, 1 << 10);

BENCHMARK_MAIN();