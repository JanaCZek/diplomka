#include <stdint.h>
#include <mutex>
#include <omp.h>
#include <tracy/Tracy.hpp>

std::atomic<int32_t> shared;
std::mutex mutex;

volatile int32_t dataA, dataB, dataC, dataD;

#define ALIGNMENT (64)

alignas(ALIGNMENT) volatile int32_t dataAlignedA;
alignas(ALIGNMENT) volatile int32_t dataAlignedB;
alignas(ALIGNMENT) volatile int32_t dataAlignedC;
alignas(ALIGNMENT) volatile int32_t dataAlignedD;

void true_sharing(int n);
void false_sharing(int n);
void false_sharing_fix(int n);

void true_sharing(int n)
{
#pragma omp parallel num_threads(4)
    for (int i = 0; i < n; i++)
    {
        mutex.lock();
        ++shared;
        mutex.unlock();
    }
}
void false_sharing(int n)
{
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        switch (thread_num)
        {
        case 0:
            for (int i = 0; i < n; i++)
            {
                ++dataA;
            }
            break;
        case 1:
            for (int i = 0; i < n; i++)
            {
                ++dataB;
            }
            break;
        case 2:
            for (int i = 0; i < n; i++)
            {
                ++dataC;
            }
            break;
        case 3:
            for (int i = 0; i < n; i++)
            {
                ++dataD;
            }
            break;
        }
    }
}
void false_sharing_fix(int n)
{
#pragma omp parallel num_threads(4)
    {
        int thread_num = omp_get_thread_num();
        switch (thread_num)
        {
        case 0:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedA;
            }
            break;
        case 1:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedB;
            }
            break;
        case 2:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedC;
            }
            break;
        case 3:
            for (int i = 0; i < n; i++)
            {
                ++dataAlignedD;
            }
            break;
        }
    }
}

int main()
{
    ZoneScoped;

    const int sizes[] = {(1 << 10), (1 << 11), (1 << 12), (1 << 13), (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18), (1 << 19), (1 << 20)};
    int sizesCount = sizeof(sizes) / sizeof(const int);

    for (int sizeIndex = 0; sizeIndex < sizesCount; sizeIndex++)
    {
        int n = sizes[sizeIndex];

        char functionName[32] = {'\0'};
        sprintf(functionName, "Size: %d", n);

        ZoneScopedN("Size");
        ZoneName(functionName, strlen(functionName));

        dataA = 0;
        dataB = 0;
        dataC = 0;
        dataD = 0;

        dataAlignedA = 0;
        dataAlignedB = 0;
        dataAlignedC = 0;
        dataAlignedD = 0;

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "False sharing, size: %d", n);

            ZoneScopedN("False sharing");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                false_sharing(n);
            }
        }

        {
            char functionName[32] = {'\0'};
            sprintf(functionName, "False sharing fix, size: %d", n);

            ZoneScopedN("False sharing fix");
            ZoneName(functionName, strlen(functionName));

            for (int repeat = 0; repeat < 997; repeat++)
            {
                false_sharing_fix(n);
            }
        }
        false_sharing(n);
        false_sharing_fix(n);
    }
}