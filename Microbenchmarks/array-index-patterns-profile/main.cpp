#include <tracy/Tracy.hpp>

#define ACCESS_COUNT (50000)

double *array;
double *arrayStrided;
double results[4] = {0.0, 0.0, 0.0, 0.0};

double sequential(int n);
double strided(int step, int n);
double random(int n);
double pattern(int n);

double sequential(int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum += array[i];
    }

    return sum;
}

double strided(int step, int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i += step)
    {
        sum += arrayStrided[i];
    }

    return sum;
}

double random(int n)
{
    double sum = 0.0;
    unsigned int seed = 42;
    srand(seed);

    for (int i = 0; i < n; i++)
    {
        int index = rand() % n;
        sum += array[index];
    }

    return sum;
}

double pattern(int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        int index = (2 * i + 5) % n;
        sum += array[index];
    }

    return sum;
}

int main()
{
    ZoneScopedS(5);

    int n = ACCESS_COUNT;

    int step = (1 << 10);
    int stridedN = ACCESS_COUNT * step;

    array = (double *)calloc(n, sizeof(double));
    TracyAlloc(array, n * sizeof(double));
    arrayStrided = (double *)calloc(stridedN, sizeof(double));
    TracyAlloc(arrayStrided, stridedN * sizeof(double));

    for (int i = 0; i < n; i++)
    {
        array[i] = 2;
    }

    for (int i = 0; i < stridedN; i++)
    {
        arrayStrided[i] = 2;
    }

    double result = 0.0;

    {
        char functionName[32] = {'\0'};
        sprintf(functionName, "Sequential, size: %d", n);

        ZoneScopedNS("Sequential", 5);
        ZoneName(functionName, strlen(functionName));

        for (int repeat = 0; repeat < 997; repeat++)
        {
            result = sequential(n);
            results[0] = result;
        }
    }

    {
        char functionName[32] = {'\0'};
        sprintf(functionName, "Pattern, size: %d", n);

        ZoneScopedNS("Pattern", 5);
        ZoneName(functionName, strlen(functionName));

        for (int repeat = 0; repeat < 997; repeat++)
        {
            result = pattern(n);
            results[1] = result;
        }
    }

    {
        char functionName[32] = {'\0'};
        sprintf(functionName, "Strided, size: %d, step: %d", n, step);

        ZoneScopedNS("Strided", 5);
        ZoneName(functionName, strlen(functionName));

        for (int repeat = 0; repeat < 997; repeat++)
        {
            result = strided(step, stridedN);
            results[2] = result;
        }
    }

    {
        char functionName[32] = {'\0'};
        sprintf(functionName, "Random, size: %d", n);

        ZoneScopedNS("Random", 5);
        ZoneName(functionName, strlen(functionName));

        for (int repeat = 0; repeat < 997; repeat++)
        {
            result = random(n);
            results[3] = result;
        }
    }

    free(array);
    TracyFree(array);
    free(arrayStrided);
    TracyFree(arrayStrided);

    return 0;
}