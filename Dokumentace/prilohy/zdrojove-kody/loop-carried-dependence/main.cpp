#include <benchmark/benchmark.h>

#define SMALL (2000)
#define MEDIUM (30000)
#define LARGE (800000)

double *A;
double *B;
double *C;
double *D;

void with_dependence();
void without_dependence();

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    A = (double *)calloc(n, sizeof(double));
    B = (double *)calloc(n, sizeof(double));
    C = (double *)calloc(n, sizeof(double));
    D = (double *)calloc(n, sizeof(double));

    for (int i = 0; i < n; i++)
    {
        A[i] = (i + 1) % 100;
        B[i] = (i + 1) % 100;
        C[i] = (i + 1) % 100;
        D[i] = (i + 1) % 100;
    }
}

static void DoTeardown(const benchmark::State &state)
{
    free(A);
    // free(B);
    free(C);
    free(D);
}

static void with_dependence(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        for (int i = 0; i < n; i++)
        {
            A[i] = A[i] + B[i];
            B[i + 1] = C[i] + D[i];
        }
    }
}

static void without_dependence(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        A[0] = A[0] + B[0];
        for (int i = 0; i < n - 2; i++)
        {
            B[i + 1] = C[i] + D[i];
            A[i + 1] = A[i + 1] + B[i + 1];
        }
        B[n - 1] = C[n - 2] + D[n - 2];
    }
}

BENCHMARK(with_dependence)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(SMALL)->Arg(MEDIUM)->Arg(LARGE);
BENCHMARK(without_dependence)->Setup(DoSetup)->Teardown(DoTeardown)->Unit(benchmark::kMillisecond)->Arg(SMALL)->Arg(MEDIUM)->Arg(LARGE);

BENCHMARK_MAIN();