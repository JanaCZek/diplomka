#include <tracy/Tracy.hpp>

double **A;
double **B;
double **C;

void ijk(int n);
void jik(int n);
void jki(int n);
void kji(int n);
void kij(int n);
void ikj(int n);

void ijk(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "IJK, size: %d", n);

    ZoneScopedN("IJK");
    ZoneName(functionName, strlen(functionName));

    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum = 0.0;

            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }
}

void jik(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "JIK, size: %d", n);

    ZoneScopedN("JIK");
    ZoneName(functionName, strlen(functionName));

    double sum = 0.0;
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            sum = 0.0;

            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }
}

void jki(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "JKI, size: %d", n);

    ZoneScopedN("JKI");
    ZoneName(functionName, strlen(functionName));

    double r = 0.0;
    for (int j = 0; j < n; j++)
    {
        for (int k = 0; k < n; k++)
        {
            r = B[k][j];

            for (int i = 0; i < n; i++)
            {
                C[i][j] += A[i][k] * r;
            }
        }
    }
}

void kji(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "KJI, size: %d", n);

    ZoneScopedN("KJI");
    ZoneName(functionName, strlen(functionName));

    double r = 0.0;
    for (int k = 0; k < n; k++)
    {
        for (int j = 0; j < n; j++)
        {
            r = B[k][j];

            for (int i = 0; i < n; i++)
            {
                C[i][j] += A[i][k] * r;
            }
        }
    }
}

void kij(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "KIJ, size: %d", n);

    ZoneScopedN("KIJ");
    ZoneName(functionName, strlen(functionName));

    double r = 0.0;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            r = A[i][k];
            for (int j = 0; j < n; j++)
            {
                C[i][j] += r * B[k][j];
            }
        }
    }
}

void ikj(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "IKJ, size: %d", n);

    ZoneScopedN("IKJ");
    ZoneName(functionName, strlen(functionName));

    double r = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < n; k++)
        {
            r = A[i][k];
            for (int j = 0; j < n; j++)
            {
                C[i][j] += r * B[k][j];
            }
        }
    }
}

int main()
{
    ZoneScopedS(5);

    const int sizes[] = {50, 100, 150, 200};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        const int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        A = (double **)calloc(n, sizeof(double *));
        B = (double **)calloc(n, sizeof(double *));
        C = (double **)calloc(n, sizeof(double *));

        for (int row = 0; row < n; row++)
        {
            A[row] = (double *)calloc(n, sizeof(double *));
            B[row] = (double *)calloc(n, sizeof(double *));
            C[row] = (double *)calloc(n, sizeof(double *));
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                A[i][j] = ((i + 1) * (j + 1)) % 100;
                B[i][j] = ((i + 1) * (j + 1)) % 100;
                C[i][j] = ((i + 1) * (j + 1)) % 100;
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "IJK, size: %d", n);

            ZoneScopedN("IJK");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                ijk(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "JIK, size: %d", n);

            ZoneScopedN("JIK");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                jik(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "JKI, size: %d", n);

            ZoneScopedN("JKI");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                jki(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "KJI, size: %d", n);

            ZoneScopedN("KJI");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                kji(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "KIJ, size: %d", n);

            ZoneScopedN("KIJ");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                kij(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "IKJ, size: %d", n);

            ZoneScopedN("IKJ");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                ikj(n);
            }
        }

        for (int row = 0; row < n; row++)
        {
            free(A[row]);
            free(B[row]);
            free(C[row]);
        }

        free(A);
        free(B);
        free(C);
    }
}