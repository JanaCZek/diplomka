#include <stdio.h>
#include <stdlib.h>
#include "benchmark/benchmark.h"

#define ROWS 1000
#define COLS ROWS

int **A;
int **B;
int **C;

void setup(int ***);
void teardown(int ***);
void print_matrix(int **);
void matrix_multiply_slow(int ***, int **, int **);
void matrix_multiply_fast(int ***, int **, int **);

// int main(void)
// {
//     setup(&A);
//     setup(&B);
//     setup(&C);

//     print_matrix(A);
//     print_matrix(B);

//     matrix_multiply_slow(&C, A, B);

//     print_matrix(C);

//     teardown(&A);
//     teardown(&B);
//     teardown(&C);

//     return 0;
// }

void setup(int ***matrix)
{
    *matrix = (int **)calloc(ROWS, sizeof(int *));

    for (int row = 0; row < COLS; row++)
    {
        (*matrix)[row] = (int *)calloc(COLS, sizeof(int));
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            (*matrix)[i][j] = ((i + 1) * (j + 1)) % 123;
        }
    }
}

void teardown(int ***matrix)
{
    for (int row = 0; row < COLS; row++)
    {
        free((*matrix)[row]);
    }

    free(*matrix);
}

void print_matrix(int **matrix)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void matrix_multiply_slow(int ***result, int **A, int **B)
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            for (int k = 0; k < ROWS; ++k)
            {
                (*result)[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_multiply_fast(int ***result, int **A, int **B)
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int k = 0; k < ROWS; ++k)
        {
            for (int j = 0; j < COLS; ++j)
            {
                (*result)[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
static void DoSetup(const benchmark::State &state)
{
    setup(&A);
    setup(&B);
    setup(&C);
}

static void DoTeardown(const benchmark::State &state)
{
    teardown(&A);
    teardown(&B);
    teardown(&C);
}

static void matrix_multiply_slow_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        matrix_multiply_slow(&C, A, B);
    }
}

static void matrix_multiply_fast_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        matrix_multiply_fast(&C, A, B);
    }
}

BENCHMARK(matrix_multiply_slow_benchmark)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);
BENCHMARK(matrix_multiply_fast_benchmark)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);

BENCHMARK_MAIN();