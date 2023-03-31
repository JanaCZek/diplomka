#include <tracy/Tracy.hpp>

#define ACCESS_COUNT (50000)

double *array;
double result;

double cache_associativity_limit(int step, int n);

double cache_associativity_limit(int step, int n)
{
    double sum = 0.0;

    for (int i = 0; i < n; i += step)
    {
        sum += array[i];
    }

    return sum;
}

int main()
{
    ZoneScoped;
    // Arg(30)->Arg(32)->Arg(60)->Arg(64)->Arg(120)->Arg(128)->Arg(250)->Arg(256)->Arg(510)->Arg(512);
    const int sizes[] = {30, 31, 32, 59, 60, 61, 64, 120, 128, 250, 256, 510, 512};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        int step = sizes[sizeIndex];
        int n = ACCESS_COUNT * step;

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        array = (double *)calloc(n, sizeof(double));
        TracyAlloc(array, n * sizeof(double));

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Cache associativity, step: %d", step);

            ZoneScopedN("Associativity step");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = cache_associativity_limit(step, n);
                result = sum;
            }
        }

        free(array);
        TracyFree(array);
    }

    return 0;
}