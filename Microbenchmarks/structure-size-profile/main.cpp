#include <stdint.h>
#include <tracy/Tracy.hpp>

int64_t results[2] = {0, 0};

struct Data
{
    bool a;
    int32_t b;
    bool c;
    int64_t d;
    bool e;
};

struct DataOrdered
{
    int64_t a;
    int32_t b;
    bool c, d, e;
};

Data *data;
DataOrdered *dataOrdered;

void data_process(int n);
void data_ordered_process(int n);

void data_process(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Data process, size: %d", n);

    ZoneScopedN("Data process");
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        data[i].d = data[i].b * data[i].b;
    }
}

void data_ordered_process(int n)
{
    char functionName[32] = {'\0'};
    sprintf(functionName, "Data ordered process, size: %d", n);

    ZoneScopedN("Data ordered process");
    ZoneName(functionName, strlen(functionName));

    for (int i = 0; i < n; i++)
    {
        dataOrdered[i].a = dataOrdered[i].b * dataOrdered[i].b;
    }
}

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

        data = (Data *)calloc(n, sizeof(Data));
        dataOrdered = (DataOrdered *)calloc(n, sizeof(DataOrdered));

        for (int i = 0; i < n; i++)
        {
            data[i].a = true;
            data[i].c = true;
            data[i].e = true;
            data[i].b = (i + 1) % 100;
            data[i].d = (i + 1) % 100;

            dataOrdered[i].a = (i + 1) % 100;
            dataOrdered[i].b = (i + 1) % 100;
            dataOrdered[i].c = true;
            dataOrdered[i].d = true;
            dataOrdered[i].e = true;
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Data process, size: %d", n);

            ZoneScopedN("Data process");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                data_process(n);

                results[0] = data[0].d;
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "Data ordered process, size: %d", n);

            ZoneScopedN("Data ordered process");
            ZoneName(functionName, strlen(functionName));

            double sum = 0.0;

            for (int repeat = 0; repeat < 997; repeat++)
            {
                data_ordered_process(n);

                results[1] = dataOrdered[0].a;
            }
        }

        free(data);
        free(dataOrdered);
    }
}