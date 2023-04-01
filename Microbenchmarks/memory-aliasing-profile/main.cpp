#include <tracy/Tracy.hpp>

double *array;

void write_read(double *src, double *dest, int n);

void write_read(double *src, double *dest, int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Write read, size: %d", n);

    ZoneScopedN("Write read");
    ZoneName(functionName, strlen(functionName));

    int count = n;
    int val = 0;

    while (count)
    {
        *dest = val;
        val = (*src) + 1;
        count--;
    }
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

        for (int i = 0; i < n; i++)
        {
            array[i] = (i + 1) % 100;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Write read, size: %d", n);

            ZoneScopedN("Write read");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                write_read(&array[0], &array[1], n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Write read aliased, size: %d", n);

            ZoneScopedN("Write read aliased");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                write_read(&array[0], &array[0], n);
            }
        }

        free(array);
    }
}