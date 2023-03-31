#include <tracy/Tracy.hpp>

double results[2] = {0.0, 0.0};

struct AoS
{
    double a, b, c, d, e, f, g, h, result;
};

struct SoA
{
    double *a, *b, *c, *d, *e, *f, *g, *h, *result;
};

AoS *dataAoS;
SoA dataSoA;

void aos(int n);
void soa(int n);

void aos(int n)
{
    for (int i = 0; i < n; i++)
    {
        dataAoS[i].result = dataAoS[i].a + dataAoS[i].b * dataAoS[i].c;
    }
}

void soa(int n)
{
    for (int i = 0; i < n; i++)
    {
        dataSoA.result[i] = dataSoA.a[i] + dataSoA.b[i] * dataSoA.c[i];
    }
}

int main()
{
    ZoneScoped;

    const int n = (1 << 20);

    dataAoS = (AoS *)calloc(n, sizeof(AoS));

    dataSoA.a = (double *)calloc(n, sizeof(double));
    dataSoA.b = (double *)calloc(n, sizeof(double));
    dataSoA.c = (double *)calloc(n, sizeof(double));
    dataSoA.d = (double *)calloc(n, sizeof(double));
    dataSoA.e = (double *)calloc(n, sizeof(double));
    dataSoA.f = (double *)calloc(n, sizeof(double));
    dataSoA.g = (double *)calloc(n, sizeof(double));
    dataSoA.h = (double *)calloc(n, sizeof(double));
    dataSoA.result = (double *)calloc(n, sizeof(double));

    for (int i = 0; i < n; i++)
    {
        dataAoS[i].a = (i + 1) % 100;
        dataAoS[i].b = (i + 1) % 100;
        dataAoS[i].c = (i + 1) % 100;
        dataAoS[i].d = (i + 1) % 100;
        dataAoS[i].e = (i + 1) % 100;
        dataAoS[i].f = (i + 1) % 100;
        dataAoS[i].g = (i + 1) % 100;
        dataAoS[i].h = (i + 1) % 100;
        dataAoS[i].result = (i + 1) % 100;

        dataSoA.a[i] = (i + 1) % 100;
        dataSoA.b[i] = (i + 1) % 100;
        dataSoA.c[i] = (i + 1) % 100;
        dataSoA.d[i] = (i + 1) % 100;
        dataSoA.e[i] = (i + 1) % 100;
        dataSoA.f[i] = (i + 1) % 100;
        dataSoA.g[i] = (i + 1) % 100;
        dataSoA.h[i] = (i + 1) % 100;
        dataSoA.result[i] = (i + 1) % 100;
    }

    {
        ZoneScopedN("AoS");

        for (int repeat = 0; repeat < 997; repeat++)
        {
            aos(n);
            results[0] = dataAoS[0].result;
        }
    }

    {
        ZoneScopedN("SoA");

        for (int repeat = 0; repeat < 997; repeat++)
        {
            soa(n);
            results[1] = dataSoA.result[0];
        }
    }

    free(dataAoS);

    free(dataSoA.a);
    free(dataSoA.b);
    free(dataSoA.c);
    free(dataSoA.d);
    free(dataSoA.e);
    free(dataSoA.f);
    free(dataSoA.g);
    free(dataSoA.h);
    free(dataSoA.result);
}