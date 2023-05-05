#include <benchmark/benchmark.h>

#define ACCESS_COUNT (10000)

double *array;
double *arrayStrided;
double results[4] = {0.0, 0.0, 0.0, 0.0};

double sequential(int n);
double strided(int step, int n);
double random(int n);
double pattern(int n);

static void DoSetup(const benchmark::State &state)
{
    int n = ACCESS_COUNT;

    int step = state.range(0);
    int stridedN = ACCESS_COUNT * step;

    array = (double *)calloc(stridedN, sizeof(double));
    arrayStrided = (double *)calloc(stridedN, sizeof(double));

    for (int i = 0; i < n; i++)
    {
        array[i] = (i + 1) % 100;
    }

    for (int i = 0; i < stridedN; i++)
    {
        arrayStrided[i] = (i + 1) % 100;
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
        int n = ACCESS_COUNT;

        double sum = sequential(n);

        // Do something with the variable so it doesn't get optimized away
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

        // Do something with the variable so it doesn't get optimized away
        results[1] = sum;
    }
}

static void random_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT;

        double sum = random(n);

        // Do something with the variable so it doesn't get optimized away
        results[2] = sum;
    }
}

static void pattern_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int step = state.range(0);
        int n = ACCESS_COUNT;

        double sum = pattern(n);

        // Do something with the variable so it doesn't get optimized away
        results[3] = sum;
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
        sum += arrayStrided[i];
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

double pattern(int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        int index = (2 * i + 5) % n;
        sum += array[index];
    }

    return sum;
}

BENCHMARK(sequential_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(pattern_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(strided_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg((1 << 1))->Arg((1 << 5))->Arg((1 << 10));
BENCHMARK(random_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(1);

BENCHMARK_MAIN();