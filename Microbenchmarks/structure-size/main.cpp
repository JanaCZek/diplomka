#include <stdint.h>
#include <benchmark/benchmark.h>

int64_t results[2] = {0, 0};

struct Data
{
    bool a;
    int32_t b;
    bool c;
    int64_t d;
    bool e;
};

struct DataOrdered
{
    int64_t a;
    int32_t b;
    bool c, d, e;
};

Data *data;
DataOrdered* dataOrdered;




static void DoSetup(const benchmark::State &state)
{
    int n = state.range(0);

    data = (Data*)calloc(n, sizeof(Data));
    dataOrdered = (DataOrdered*)calloc(n, sizeof(DataOrdered));

    for (int i = 0; i < n; i++)
    {
        data[i].a = true;
        data[i].c = true;
        data[i].e = true;
        data[i].b = (i + 1) % 100;
        data[i].d = (i + 1) % 100;

        dataOrdered[i].a = (i + 1) % 100;
        dataOrdered[i].b = (i + 1) % 100;
        dataOrdered[i].c = true;
        dataOrdered[i].d = true;
        dataOrdered[i].e = true;
    }
    
}

static void DoTeardown(const benchmark::State &state)
{
    free(data);
    free(dataOrdered);
}

static void data_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        for (int i = 0; i < n; i++)
        {
            data[i].d = data[i].b * data[i].b;
        }

        // Do something with the variable so it doesn't get optimized away
        results[0] = data[0].d;
    }
}

static void data_ordered_benchmark(benchmark::State &state)
{
    for (auto _ : state)
    {
        int n = state.range(0);

        for (int i = 0; i < n; i++)
        {
            dataOrdered[i].a = dataOrdered[i].b * dataOrdered[i].b;
        }

        // Do something with the variable so it doesn't get optimized away
        results[1] = dataOrdered[1].a;
    }
}

#define START (1 << 10)
#define END (1 << 20)

BENCHMARK(data_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(data_ordered_benchmark)->Setup(DoSetup)->Teardown(DoTeardown)->RangeMultiplier(2)->Range(START, END);

BENCHMARK_MAIN();