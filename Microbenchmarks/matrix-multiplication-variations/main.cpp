#include <benchmark/benchmark.h>

double **A;
double **B;
double **C;

void setup();
void teardown();
void ijk();
void jik();
void jki();
void kji();
void kij();
void ikj();

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    A = (double **)calloc(n, sizeof(double *));
    B = (double **)calloc(n, sizeof(double *));
    C = (double **)calloc(n, sizeof(double *));

    for (int row = 0; row < n; row++)
    {
        A[row] = (double *)calloc(n, sizeof(double *));
        B[row] = (double *)calloc(n, sizeof(double *));
        C[row] = (double *)calloc(n, sizeof(double *));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = ((i + 1) * (j + 1)) % 100;
            B[i][j] = ((i + 1) * (j + 1)) % 100;
            C[i][j] = ((i + 1) * (j + 1)) % 100;
        }
    }
}

static void DoTeardown(const benchmark::State &state)
{
    int n = state.range(0);

    for (int row = 0; row < n; row++)
    {
        free(A[row]);
        free(B[row]);
        free(C[row]);
    }

    free(A);
    free(B);
    free(C);
}

static void ijk(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = 0.0;
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
    }
}

static void jik(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double sum = 0.0;
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
    }
}

static void jki(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

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
    }
}

static void kji(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double r = 0.0;
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
    }
}

static void kij(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double r = 0.0;
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
    }
}

static void ikj(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        double r = 0.0;
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
}

#define N1 50
#define N2 100

BENCHMARK(ijk)->Setup(DoSetup)->Arg(N1)->Arg(N2);
BENCHMARK(jik)->Arg(N1)->Arg(N2);
BENCHMARK(jki)->Arg(N1)->Arg(N2);
BENCHMARK(kji)->Arg(N1)->Arg(N2);
BENCHMARK(kij)->Arg(N1)->Arg(N2);
BENCHMARK(ikj)->Teardown(DoTeardown)->Arg(N1)->Arg(N2);

BENCHMARK_MAIN();