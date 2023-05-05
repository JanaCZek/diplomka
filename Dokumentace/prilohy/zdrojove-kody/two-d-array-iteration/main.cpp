#include <benchmark/benchmark.h>

#define SMALL (45)
#define MEDIUM (170)
#define LARGE (900)

double **array;
double results[2] = {0.0, 0.0};

void rows_cols();
void cols_rows();

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    array = (double **)calloc(n, sizeof(double *));

    for (int row = 0; row < n; row++)
    {
        array[row] = (double *)calloc(n, sizeof(double *));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            array[i][j] = ((i + 1) * (j + 1)) % 100;
        }
    }
}

static void DoTeardown(const benchmark::State &state)
{
    int n = state.range(0);

    for (int row = 0; row < n; row++)
    {
        free(array[row]);
    }

    free(array);
}

static void rows_cols(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                sum += array[i][j];
            }
        }

        // Do something with the variable so it doesn't get optimized away
        results[0] = sum;
    }
}

static void cols_rows(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = 0;
        for (int j = 0; j < n; j++)
        {
            for (int i = 0; i < n; i++)
            {
                sum += array[i][j];
            }
        }

        // Do something with the variable so it doesn't get optimized away
        results[1] = sum;
    }
}

BENCHMARK(rows_cols)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(SMALL)->Arg(MEDIUM)->Arg(LARGE);
BENCHMARK(cols_rows)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(SMALL)->Arg(MEDIUM)->Arg(LARGE);

BENCHMARK_MAIN();