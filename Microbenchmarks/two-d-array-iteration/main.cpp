#include <benchmark/benchmark.h>
#include <tracy/Tracy.hpp>

#define N (1 << 11)
#define ROWS N
#define COLS N

double **array;
double results[2] = {0.0, 0.0};

void rows_cols();
void cols_rows();

static void DoSetup(const benchmark::State &state)
{
    array = (double **)calloc(ROWS, sizeof(double *));

    for (int row = 0; row < COLS; row++)
    {
        array[row] = (double *)calloc(COLS, sizeof(double *));
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            array[i][j] = ((i + 1) * (j + 1)) % 100;
        }
    }
}

static void DoTeardown(const benchmark::State &state)
{
    for (int row = 0; row < COLS; row++)
    {
        free(array[row]);
    }

    free(array);
}

static void rows_cols(benchmark::State &state)
{
    for (auto _ : state)
    {
        double sum = 0;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
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
        double sum = 0;
        for (int j = 0; j < COLS; j++)
        {
            for (int i = 0; i < ROWS; i++)
            {
                sum += array[i][j];
            }
        }

        // Do something with the variable so it doesn't get optimized away
        results[1] = sum;
    }
}

BENCHMARK(rows_cols)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(cols_rows)->Setup(DoSetup)->Teardown(DoTeardown);

BENCHMARK_MAIN();