// #define TRACY_ON 1

#ifdef TRACY_ON
#include <tracy/Tracy.hpp>
#endif

#ifndef TRACY_ON
#include <stdlib.h>
#endif

#define SMALL (2000)
#define MEDIUM (30000)
#define LARGE (800000)

double *array;
double results[3] = {0.0, 0.0, 0.0};

void array_sum(double *array, int n, double *result);
void array_sum_unrolled_2(double *array, int n, double *result);
void array_sum_unrolled_4(double *array, int n, double *result);

void array_sum(double *array, int n, double *result)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum, size: %d", n);

    ZoneScopedNS("Array sum", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    for (int i = 0; i < n; i++)
    {
        *result += array[i];
    }
}
void array_sum_unrolled_2(double *array, int n, double *result)
{
#ifdef TRACY_ON
    char functionName[64] = {'\0'};
    sprintf(functionName, "Array sum unrolled 2, size: %d", n);

    ZoneScopedNS("Array sum unrolled 2", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    int i = 0;
    int limit = n - 1;
    double accumulator = 0.0;

    // Unrolled loop
    for (; i < limit; i += 2)
    {
        accumulator += array[i] + array[i + 1];
    }

    // Finalizing loop
    for (; i < n; i++)
    {
        accumulator += array[i];
    }

    *result = accumulator;
}

void array_sum_unrolled_4(double *array, int n, double *result)
{
#ifdef TRACY_ON
    char functionName[64] = {'\0'};
    sprintf(functionName, "Array sum unrolled 4, size: %d", n);

    ZoneScopedNS("Array sum unrolled 4", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    int i = 0;
    int limit = n - 3;
    double accumulator = 0.0;

    // Unrolled loop
    for (; i < limit; i += 4)
    {
        accumulator += array[i] + array[i + 1] + array[i + 2] + array[i + 3];
    }

    // Finalizing loop
    for (; i < n; i++)
    {
        accumulator += array[i];
    }

    *result = accumulator;
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
        char functionName[64] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedNS("Size", 5);
        ZoneName(functionName, strlen(functionName));
#endif
        array = (double *)calloc(n, sizeof(double));

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
            double sum = 0.0;
#ifdef TRACY_ON
            char functionName[64] = {'\0'};
            sprintf(functionName, "Array sum, size: %d", n);

            ZoneScopedNS("Array sum", 5);
            ZoneName(functionName, strlen(functionName));
            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 18997; repeat++)
#endif
            {
                array_sum(array, n, &sum);
            }
        }

        {
            double sum = 0.0;
#ifdef TRACY_ON
            char functionName[64] = {'\0'};
            sprintf(functionName, "Array sum unrolled 2, size: %d", n);

            ZoneScopedNS("Array sum unrolled 2", 5);
            ZoneName(functionName, strlen(functionName));
            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 18997; repeat++)
#endif
            {
                array_sum_unrolled_2(array, n, &sum);
            }
        }

        {
            double sum = 0.0;
#ifdef TRACY_ON
            char functionName[64] = {'\0'};
            sprintf(functionName, "Array sum unrolled 4, size: %d", n);

            ZoneScopedNS("Array sum unrolled 4", 5);
            ZoneName(functionName, strlen(functionName));
            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 18997; repeat++)
#endif
            {
                array_sum_unrolled_4(array, n, &sum);
            }
        }

        free(array);
    }
}