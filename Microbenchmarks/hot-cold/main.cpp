#include <benchmark/benchmark.h>
#include <tracy/Tracy.hpp>

double results[2] = {0.0, 0.0};

struct Data
{
    double a, b, c, d, e, f, g, h, result;
};

struct DataHot
{
    double a, b, c, result;
};

struct DataCold
{
    double d, e, f, g, h;
};

struct DataHotCold
{
    DataHot *hot;
    DataCold *cold;
};

Data *data;
DataHotCold dataHotCold;

void together(int n);
void separated(int n);

static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    data = (Data *)calloc(n, sizeof(Data));
    dataHotCold.hot = (DataHot *)calloc(n, sizeof(DataHot));
    dataHotCold.cold = (DataCold *)calloc(n, sizeof(DataCold));

    for (int i = 0; i < n; i++)
    {
        data[i].a = (i + 1) % 100;
        data[i].b = (i + 1) % 100;
        data[i].c = (i + 1) % 100;
        data[i].d = (i + 1) % 100;
        data[i].e = (i + 1) % 100;
        data[i].f = (i + 1) % 100;
        data[i].g = (i + 1) % 100;
        data[i].h = (i + 1) % 100;
        data[i].result = (i + 1) % 100;

        dataHotCold.hot[i].a = (i + 1) % 100;
        dataHotCold.hot[i].b = (i + 1) % 100;
        dataHotCold.hot[i].c = (i + 1) % 100;
        dataHotCold.hot[i].result = (i + 1) % 100;

        dataHotCold.cold[i].d = (i + 1) % 100;
        dataHotCold.cold[i].e = (i + 1) % 100;
        dataHotCold.cold[i].f = (i + 1) % 100;
        dataHotCold.cold[i].g = (i + 1) % 100;
        dataHotCold.cold[i].h = (i + 1) % 100;
    }
}

static void DoTeardown(const benchmark::State &state)
{
    free(data);
    free(dataHotCold.hot);
    free(dataHotCold.cold);
}

static void together_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        together(n);

        // Do something with the variable so it doesn't get optimized away
        results[0] = data[0].result;
    }
}

static void separated_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        separated(n);

        // Do something with the variable so it doesn't get optimized away
        results[1] = dataHotCold.hot->result;
    }
}

void together(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Together, size: %d", n);

    ZoneScopedS(5);
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        data[i].result = data[i].a + data[i].b * data[i].c;
    }
}

void separated(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Separated, size: %d", n);

    ZoneScopedS(5);
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        dataHotCold.hot[i].result = dataHotCold.hot[i].a + dataHotCold.hot[i].b * dataHotCold.hot[i].c;
    }
}

#define START (1 << 10)
#define END (1 << 20)

BENCHMARK(together_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(separated_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);

BENCHMARK_MAIN();

// int main()
// {
//     ZoneScopedS(5);

//     const int sizes[] = {(1 << 10), (1 << 11), (1 << 12), (1 << 13), (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18), (1 << 19), (1 << 20)};
//     int sizesCount = sizeof(sizes) / sizeof(const int);

//     for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
//     {
//         const int n = sizes[sizeIndex];

//         data = (Data *)calloc(n, sizeof(Data));
//         dataHotCold.hot = (DataHot *)calloc(n, sizeof(DataHot));
//         dataHotCold.cold = (DataCold *)calloc(n, sizeof(DataCold));

//         for (int i = 0; i < n; i++)
//         {
//             data[i].a = (i + 1) % 100;
//             data[i].b = (i + 1) % 100;
//             data[i].c = (i + 1) % 100;
//             data[i].d = (i + 1) % 100;
//             data[i].e = (i + 1) % 100;
//             data[i].f = (i + 1) % 100;
//             data[i].g = (i + 1) % 100;
//             data[i].h = (i + 1) % 100;
//             data[i].result = (i + 1) % 100;

//             dataHotCold.hot[i].a = (i + 1) % 100;
//             dataHotCold.hot[i].b = (i + 1) % 100;
//             dataHotCold.hot[i].c = (i + 1) % 100;
//             dataHotCold.hot[i].result = (i + 1) % 100;

//             dataHotCold.cold[i].d = (i + 1) % 100;
//             dataHotCold.cold[i].e = (i + 1) % 100;
//             dataHotCold.cold[i].f = (i + 1) % 100;
//             dataHotCold.cold[i].g = (i + 1) % 100;
//             dataHotCold.cold[i].h = (i + 1) % 100;
//         }

//         together(n);

//         // Do something with the variable so it doesn't get optimized away
//         results[0] = data[0].result;

//         separated(n);

//         // Do something with the variable so it doesn't get optimized away
//         results[1] = dataHotCold.hot->result;

//         free(data);
//         free(dataHotCold.hot);
//         free(dataHotCold.cold);
//     }
// }