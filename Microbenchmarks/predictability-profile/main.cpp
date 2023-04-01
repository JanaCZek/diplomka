#include <tracy/Tracy.hpp>

#define SEED (0)

double *array;
double results[3] = {0.0, 0.0, 0.0};

double never(const double *array, int n);
double always(const double *array, int n);
double not_so_predictable(const double *array, int n);

double never(const double *array, int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Never, size: %d", n);

    ZoneScopedN("Never");
    ZoneName(functionName, strlen(functionName));

    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] > 20.0)
        {
            result = array[i];
        }
    }

    return result;
}

double always(const double *array, int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Always, size: %d", n);

    ZoneScopedN("Always");
    ZoneName(functionName, strlen(functionName));

    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] >= 0.0 && array[i] < 10.0)
        {
            result = array[i];
        }
    }

    return result;
}

double not_so_predictable(const double *array, int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Not so predictable, size: %d", n);

    ZoneScopedN("Not so predictable");
    ZoneName(functionName, strlen(functionName));

    double result = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (array[i] > 5.0)
        {
            result = array[i];
        }
    }

    return result;
}
int main()
{
    ZoneScopedS(5);

    const int sizes[] = {(1 << 18), (1 << 19), (1 << 20)};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        array = (double *)calloc(n, sizeof(double));

        srand(SEED);

        for (int i = 0; i < n; i++)
        {
            array[i] = rand() % 10;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Never, size: %d", n);

            ZoneScopedN("Never");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                results[0] = never(array, n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Always, size: %d", n);

            ZoneScopedN("Always");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                results[1] = always(array, n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Not so predictable, size: %d", n);

            ZoneScopedN("Not so predictable");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                results[2] = not_so_predictable(array, n);
            }
        }

        free(array);
    }
}