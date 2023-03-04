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
void matrix_multiplications();
void loop_dependence();
void loop_unrolling_slow(double* array, int n, double result);
void loop_unrolling_fast(double* array, int n, double result);

int main(void)
{
    int **array;

    // Array initialization

    int sum = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            sum += array[i][j];
        }
    }

    int sum = 0;
    for (int j = 0; j < COLS; j++)
    {
        for (int i = 0; i < ROWS; i++)
        {
            sum += array[i][j];
        }
    }

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

void matrix_multiplications()
{

    double **A;
    double **B;
    double **C;

    double sum = 0.0;
    int n = 10;

    // Version 1
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum = 0.0;

            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }

    // Version 2
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            sum = 0.0;

            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }

    // Version 3
    double r = 0.0;
    for (int j = 0; j < n; j++)
    {
        for (int k = 0; k < n; k++)
        {
            r = B[k][j];
            for (int i = 0; i < n; i++)
            {
                C[i][j] += A[i][k] * r;
            }
        }
    }

    // Version 4
    for (int k = 0; k < n; k++)
    {
        for (int j = 0; j < n; j++)
        {
            r = B[k][j];
            for (int i = 0; i < n; i++)
            {
                C[i][j] += A[i][k] * r;
            }
        }
    }

    // Version 5
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            r = A[i][k];
            for (int j = 0; j < n; j++)
            {
                C[i][j] += r * B[k][j];
            }
        }
    }

    // Version 6
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < n; k++)
        {
            r = A[i][k];
            for (int j = 0; j < n; j++)
            {
                C[i][j] += r * B[k][j];
            }
        }
    }
}

void loop_dependence()
{
    double *A;
    double *B;
    double *C;
    double *D;

    int n = 10;// A number big enough to show the impact

    // With dependence
    for (int i = 0; i < n; i++)
    {
        A[i] = A[i] + B[i];
        B[i+1] = C[i] + D[i];
    }
    
    // Without dependence
    A[0] = A[0] + B[0];
    for (int i = 0; i < n - 1; i++)
    {
        B[i+1] = C[i] + D[i];
        A[i+1] = A[i+1] + B[i+1];
    }
    B[n] = C[n-1] + D[n-1];
}

void loop_unrolling_slow(double* array, int n, double* result) {

    for (int i = 0; i < n; i++)
    {
        *result += array[i];
    }
}

void loop_unrolling_fast(double* array, int n, double* result) {

    int i = 0;
    int limit = n - 1;
    double accumulator = 0.0;

    // Unrolled loop
    for (; i < limit; i+=2)
    {
        accumulator += array[i];
        accumulator += array[i+1];
    }

    // Finalizing loop
    for (; i < n; i++)
    {
        accumulator += array[i];
    }
    
    *result = accumulator;
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