#include <stdio.h>
#include <stdlib.h>
#include <tracy/Tracy.hpp>
// #include "benchmark/benchmark.h"

#define ROWS 5000
#define COLS 5000

int **arr;

void setup();
void teardown();
void rows_cols();
void cols_rows();

int main(void)
{
    ZoneScopedS(5);

    setup();

    rows_cols();

    cols_rows();

    teardown();

    return 0;
}

void setup()
{
    ZoneScopedS(5);
    arr = (int **)calloc(ROWS, sizeof(int *));
    TracyAlloc(arr, sizeof(int *));

    for (int row = 0; row < COLS; row++)
    {
        arr[row] = (int *)calloc(COLS, sizeof(int *));
        TracyAlloc(arr[row], sizeof(int *));
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            arr[i][j] = ((i + 1) * (j + 1)) % 123;
        }
    }
}
void teardown()
{
    ZoneScopedS(5);
    for (int row = 0; row < COLS; row++)
    {
        TracyFree(arr[row]);
        free(arr[row]);
    }

    free(arr);
}
void rows_cols()
{
    ZoneScopedS(5);
    int sum = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            sum += arr[i][j];
        }
    }
}
void cols_rows()
{
    ZoneScopedS(5);
    int sum = 0;
    for (int j = 0; j < COLS; j++)
    {
        for (int i = 0; i < ROWS; i++)
        {
            sum += arr[i][j];
        }
    }
}

// static void DoSetup(const benchmark::State &state)
// {
//     arr = (int **)calloc(ROWS, sizeof(int *));

//     for (int row = 0; row < COLS; row++)
//     {
//         arr[row] = (int *)calloc(COLS, sizeof(int *));
//     }

//     for (int i = 0; i < ROWS; i++)
//     {
//         for (int j = 0; j < COLS; j++)
//         {
//             arr[i][j] = ((i + 1) * (j + 1)) % 123;
//         }
//     }
// }

// static void DoTeardown(const benchmark::State &state)
// {
//     for (int row = 0; row < COLS; row++)
//     {
//         free(arr[row]);
//     }

//     free(arr);
// }

// static void rows_cols(benchmark::State &state)
// {
//     for (auto _ : state)
//     {
//         int sum = 0;
//         for (int i = 0; i < ROWS; i++)
//         {
//             for (int j = 0; j < COLS; j++)
//             {
//                 sum += arr[i][j];
//             }
//         }
//     }
// }

// static void cols_rows(benchmark::State &state)
// {
//     for (auto _ : state)
//     {
//         int sum = 0;
//         for (int j = 0; j < COLS; j++)
//         {
//             for (int i = 0; i < ROWS; i++)
//             {
//                 sum += arr[i][j];
//             }
//         }
//     }
// }

// BENCHMARK(rows_cols)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);
// BENCHMARK(cols_rows)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);

// BENCHMARK_MAIN();