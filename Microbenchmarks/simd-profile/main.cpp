// #define TRACY_ON 1

#ifdef TRACY_ON
#include <tracy/Tracy.hpp>
#endif

#ifndef TRACY_ON
#include <stdlib.h>
#endif

#include <intrin.h>

#define SMALL (2000)
#define MEDIUM (30000)
#define LARGE (800000)

double *array;
double results[2] = {0.0, 0.0};

double array_sum(int n);
double array_sum_simd(int n);

double array_sum(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum, size: %d", n);

    ZoneScopedNS("Array sum", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }

    return sum;
}

double array_sum_simd(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum SIMD, size: %d", n);

    ZoneScopedNS("Array sum SIMD", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    // SSE specific step
    int step = 128 / (sizeof(double) * 8);
    int sumSize = step;
    double *sum = (double *)malloc(sizeof(double) * sumSize);

    __m128d sumVector = _mm_loadu_pd(sum);
    __m128d *arrayVector = (__m128d *)array;
    int vecIterations = n / step;

    for (int i = 0; i < vecIterations; i++)
    {
        sumVector = _mm_add_pd(arrayVector[i], sumVector);
    }

    _mm_store_pd(sum, sumVector);

    double sumTotal = 0.0;

    for (int i = 0; i < sumSize; i++)
    {
        sumTotal += sum[i];
    }

    return sumTotal;
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

        ZoneScopedNS("Size", 5);
        ZoneName(functionName, strlen(functionName));
#endif
        array = (double *)_aligned_malloc(sizeof(double) * n, 16);

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum, size: %d", n);

            ZoneScopedNS("Array sum", 5);
            ZoneName(functionName, strlen(functionName));
#endif
            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = array_sum(n);

                // Do something with the variable so it doesn't get optimized away
                results[0] = sum;
            }
        }

        {
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum SIMD, size: %d", n);

            ZoneScopedNS("Array sum SIMD", 5);
            ZoneName(functionName, strlen(functionName));
#endif
            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = array_sum_simd(n);

                // Do something with the variable so it doesn't get optimized away
                results[1] = sum;
            }
        }
    }
}