#include <tracy/Tracy.hpp>

double *A;
double *B;
double *C;
double *D;

void with_dependence(int n);
void without_dependence(int n);

void with_dependence(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "With dependence, size: %d", n);

    ZoneScopedNS("With dependence", 5);
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        A[i] = A[i] + B[i];
        B[i + 1] = C[i] + D[i];
    }
}

void without_dependence(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Without dependence, size: %d", n);

    ZoneScopedNS("Without dependence", 5);
    ZoneName(functionName, strlen(functionName));

    A[0] = A[0] + B[0];
    for (int i = 0; i < n - 2; i++)
    {
        B[i + 1] = C[i] + D[i];
        A[i + 1] = A[i + 1] + B[i + 1];
    }
    B[n - 1] = C[n - 2] + D[n - 2];
}

int main()
{
    ZoneScopedS(5);

    const int sizes[] = {(1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5), (1 << 6), (1 << 7), (1 << 8), (1 << 9), (1 << 10), (1 << 11), (1 << 12), (1 << 13), (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18), (1 << 19), (1 << 20)};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        A = (double *)calloc(n, sizeof(double));
        B = (double *)calloc(n, sizeof(double));
        C = (double *)calloc(n, sizeof(double));
        D = (double *)calloc(n, sizeof(double));

        for (int i = 0; i < n; i++)
        {
            A[i] = (i + 1) % 100;
            B[i] = (i + 1) % 100;
            C[i] = (i + 1) % 100;
            D[i] = (i + 1) % 100;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "With dependence, size: %d", n);

            ZoneScopedNS("With dependence", 5);
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                with_dependence(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Without dependence, size: %d", n);

            ZoneScopedNS("Without dependence", 5);
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                without_dependence(n);
            }
        }

        free(A);
        // free(B);
        free(C);
        free(D);
    }
}