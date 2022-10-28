#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include "benchmark/benchmark.h"

#define N 10000

class GameObject {
    float m_Pos[20];
    float m_A[100];
    float m_B[100];
    float m_C[100];
    char m_Name[100];
public:
    float m_Foo;
    float m_Velocity[2];
    void updateFoo(float f) {
        float mag = sqrtf(m_Velocity[0] * m_Velocity[0] + m_Velocity[1] * m_Velocity[1]);
        m_Foo += mag * f;
    }
};

typedef struct {
    float m_Velocity[2];
    float m_Foo;
} FooUpdateIn;

typedef struct {
    float m_Foo;
} FooUpdateOut;

typedef struct {
    char c;
    double d;
    short s;
    int i;
} Larger;

typedef struct {
    char c;
    short s;
    int i;
    double d;
} Smaller;

void setup();
void teardown();
void updateFoos(const FooUpdateIn*, size_t, FooUpdateOut*, float);

GameObject* objects;
FooUpdateIn* ins;
FooUpdateOut* outs;

int main(void)
{
    //setup();

    // for (int i = 0; i < N; i++)
    // {
    //     objects[i].updateFoo(1.5f);
    // }
    
    // updateFoos(ins, N, outs, 1.5);

    printf("Align Smaller: %d\n", alignof(Smaller));
    printf("Align Larger: %d\n", alignof(Larger));
    printf("Smaller: %d\n", sizeof(Smaller));
    printf("Larger: %d\n", sizeof(Larger));

    //teardown();

    return 0;
}

void setup()
{
    objects = (GameObject*)calloc(N, sizeof(GameObject));
    ins = (FooUpdateIn*)calloc(N, sizeof(FooUpdateIn));
    outs = (FooUpdateOut*)calloc(N, sizeof(FooUpdateOut));

    for (size_t i = 0; i < N; i++)
    {
        objects[i].m_Velocity[0] = rand() % 10;
        objects[i].m_Velocity[1] = rand() % 10;
        objects[i].m_Foo = rand() % 10;

        ins[i].m_Velocity[0] = rand() % 10;
        ins[i].m_Velocity[1] = rand() % 10;
        ins[i].m_Foo = rand() % 10;
    }
    
}
void teardown()
{
    free(objects);
    free(ins);
    free(outs);
}

void updateFoos(const FooUpdateIn* in, size_t count, FooUpdateOut* out, float f) {
    for (size_t i = 0; i < count; i++)
    {
        FooUpdateIn inCurr = in[i];
        FooUpdateOut outCurr = out[i];
        float mag = sqrtf(inCurr.m_Velocity[0] * inCurr.m_Velocity[0] + inCurr.m_Velocity[1] * inCurr.m_Velocity[1]);
        outCurr.m_Foo = inCurr.m_Foo * mag;

        out[i] = outCurr;
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