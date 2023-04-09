#define TRACY_ON 1

#ifdef TRACY_ON
#include <tracy/Tracy.hpp>
#endif

#ifndef TRACY_ON
#include <stdlib.h>
#endif

#define SMALL (2000)
#define MEDIUM (30000)
#define LARGE (800000)

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

void together(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Together, size: %d", n);

    ZoneScopedNS("Together", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    for (int i = 0; i < n; i++)
    {
        data[i].result = data[i].a + data[i].b * data[i].c;
    }
}

void separated(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Separated, size: %d", n);

    ZoneScopedNS("Separated", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    for (int i = 0; i < n; i++)
    {
        dataHotCold.hot[i].result = dataHotCold.hot[i].a + dataHotCold.hot[i].b * dataHotCold.hot[i].c;
    }
}

int main()
{
#ifdef TRACY_ON
    ZoneScopedS(5);
#endif
    const int sizes[] = {SMALL, MEDIUM, LARGE};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

#ifdef TRACY_ON
        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));
#endif
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

        {
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Together, size: %d", n);

            ZoneScopedN("Together");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
             for (int repeat = 0; repeat < 2997; repeat++)
#endif
            {
                together(n);

                // Do something with the variable so it doesn't get optimized away
                results[0] = data[0].result;
            }
        }

        {
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Separated, size: %d", n);

            ZoneScopedN("Separated");
            ZoneName(functionName, strlen(functionName));
            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 2997; repeat++)
#endif
            {
                separated(n);

                // Do something with the variable so it doesn't get optimized away
                results[1] = dataHotCold.hot->result;
            }
        }

        free(data);
        free(dataHotCold.hot);
        free(dataHotCold.cold);
    }
}