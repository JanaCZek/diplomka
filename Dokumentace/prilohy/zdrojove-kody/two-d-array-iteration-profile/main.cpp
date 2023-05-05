#define TRACY_ON 1

#ifdef TRACY_ON
#include <tracy/Tracy.hpp>
#endif

#ifndef TRACY_ON
#include <stdlib.h>
#endif

#define SMALL (45)
#define MEDIUM (170)
#define LARGE (900)

double **array;
double results[2] = {0.0, 0.0};

double rows_cols(int n);
double cols_rows(int n);

double rows_cols(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Rows first, size: %d", n);

    ZoneScopedNS("Rows first", 5);
    ZoneName(functionName, strlen(functionName));
#endif

    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum += array[i][j];
        }
    }

    return sum;
}

double cols_rows(int n)
{
#ifdef TRACY_ON
    char functionName[32] = {'\0'};
    sprintf(functionName, "Columns first, size: %d", n);

    ZoneScopedNS("Columns first", 5);
    ZoneName(functionName, strlen(functionName));
#endif
    double sum = 0;
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            sum += array[i][j];
        }
    }

    return sum;
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
        array = (double **)calloc(n, sizeof(double *));
#ifdef TRACY_ON
        TracyAlloc(array, n * sizeof(double));
#endif
        for (int row = 0; row < n; row++)
        {
            array[row] = (double *)calloc(n, sizeof(double *));
#ifdef TRACY_ON
            TracyAlloc(array[row], n * sizeof(double));
#endif
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                array[i][j] = ((i + 1) * (j + 1)) % 100;
            }
        }

        {
            double sum = 0.0;
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Rows first, size: %d", n);

            ZoneScopedNS("Rows first", 5);
            ZoneName(functionName, strlen(functionName));
            for (int repeat = 0; repeat < 997; repeat++)
#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 4997; repeat++)
#endif
            {
                sum = rows_cols(n);
                results[0] = sum;
            }
        }

        {
            double sum = 0.0;
#ifdef TRACY_ON
            char functionName[32] = {'\0'};
            sprintf(functionName, "Columns first, size: %d", n);

            ZoneScopedNS("Columns first", 5);
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)

#endif
#ifndef TRACY_ON
            for (int repeat = 0; repeat < 4997; repeat++)
#endif
            {
                sum = cols_rows(n);
                results[1] = sum;
            }
        }

        for (int row = 0; row < n; row++)
        {
#ifdef TRACY_ON
            TracyFree(array[row]);
#endif
            free(array[row]);
        }
#ifdef TRACY_ON
        TracyFree(array);
#endif
        free(array);
    }
}