#include <tracy/Tracy.hpp>

double **array;
double results[2] = {0.0, 0.0};

double rows_cols(int n);
double cols_rows(int n);

double rows_cols(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Rows first, size: %d", n);

    ZoneScopedN("Rows first");
    ZoneName(functionName, strlen(functionName));

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
    char functionName[32] = {'\0'};
    sprintf(functionName, "Columns first, size: %d", n);

    ZoneScopedN("Columns first");
    ZoneName(functionName, strlen(functionName));

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
    ZoneScopedS(5);

    const int sizes[] = {(1 << 8), (1 << 9), (1 << 10), (1 << 11)};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        array = (double **)calloc(n, sizeof(double *));
        TracyAlloc(array, n * sizeof(double));

        for (int row = 0; row < n; row++)
        {
            array[row] = (double *)calloc(n, sizeof(double *));
            TracyAlloc(array[row], n * sizeof(double));
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                array[i][j] = ((i + 1) * (j + 1)) % 100;
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Rows first, size: %d", n);

            ZoneScopedN("Rows first");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = rows_cols(n);
                results[0] = sum;
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Columns first, size: %d", n);

            ZoneScopedN("Columns first");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                sum = cols_rows(n);
                results[1] = sum;
            }
        }

        for (int row = 0; row < n; row++)
        {
            TracyFree(array[row]);
            free(array[row]);
        }

        TracyFree(array);
        free(array);
    }
}