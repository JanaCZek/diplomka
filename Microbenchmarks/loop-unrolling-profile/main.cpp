#include <tracy/Tracy.hpp>

double *array;
double results[3] = {0.0, 0.0, 0.0};

void array_sum(double *array, int n, double *result);
void array_sum_unrolled_2(double *array, int n, double *result);
void array_sum_unrolled_4(double *array, int n, double *result);

void array_sum(double *array, int n, double *result)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum: %d", n);

    ZoneScopedN("Array sum");
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        *result += array[i];
    }
}
void array_sum_unrolled_2(double *array, int n, double *result)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum unrolled 2: %d", n);

    ZoneScopedN("Array sum unrolled 2");
    ZoneName(functionName, strlen(functionName));

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
    char functionName[32] = {'\0'};
    sprintf(functionName, "Array sum unrolled 4: %d", n);

    ZoneScopedN("Array sum unrolled 4");
    ZoneName(functionName, strlen(functionName));

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

#define START (1 << 4)
#define END (1 << 15)

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

        array = (double *)calloc(n, sizeof(double));

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum: %d", n);

            ZoneScopedN("Array sum");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                array_sum(array, n, &sum);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum unrolled 2: %d", n);

            ZoneScopedN("Array sum unrolled 2");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                array_sum_unrolled_2(array, n, &sum);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Array sum unrolled 4: %d", n);

            ZoneScopedN("Array sum unrolled 4");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                array_sum_unrolled_4(array, n, &sum);
            }
        }

        free(array);
    }
}