#include <intrin.h>
#include <tracy/Tracy.hpp>

double *array;
double results[2] = {0.0, 0.0};

double array_sum(int n);
double array_sum_simd(int n);

double array_sum(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum, size: %d", n);

    ZoneScopedN("Array sum");
    ZoneName(functionName, strlen(functionName));

    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }

    return sum;
}

double array_sum_simd(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum SIMD, size: %d", n);

    ZoneScopedN("Array sum SIMD");
    ZoneName(functionName, strlen(functionName));

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

#define START (1 << 10)
#define END (1 << 20)

int main()
{
    ZoneScopedS(5);

    const int sizes[] = {(1 << 10), (1 << 11), (1 << 12), (1 << 13), (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18), (1 << 19), (1 << 20)};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        array = (double *)_aligned_malloc(sizeof(double) * n, 16);

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum, size: %d", n);

            ZoneScopedN("Array sum");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = array_sum(n);

                // Do something with the variable so it doesn't get optimized away
                results[0] = sum;
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum SIMD, size: %d", n);

            ZoneScopedN("Array sum SIMD");
            ZoneName(functionName, strlen(functionName));

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