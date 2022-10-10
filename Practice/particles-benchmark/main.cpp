#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include "benchmark/benchmark.h"

#define N 20000

struct float_3
{
    float x;
    float y;
    float z;
};

struct Particle
{
    float_3 pos;
    float_3 vel;
    float_3 acc;
};

Particle *particles;

void allocate(Particle **);
void setup(Particle **);
void calculation_slow(Particle **);
void calculation_fast(Particle **);
void teardown(Particle **);

int main(void)
{
    allocate(&particles);

    setup(&particles);

    calculation_slow(&particles);

    setup(&particles);

    calculation_fast(&particles);

    teardown(&particles);

    return 0;
}

void allocate(Particle **particles)
{
    *particles = (Particle *)calloc(N, sizeof(Particle));
}

void setup(Particle **particles)
{
    for (int i = 0; i < N; i++)
    {
        Particle particle;
        particle.acc.x = rand() % 3;
        particle.acc.y = rand() % 3;
        particle.acc.z = rand() % 3;

        particle.pos.x = rand() % 3;
        particle.pos.y = rand() % 3;
        particle.pos.z = rand() % 3;

        particle.vel.x = rand() % 3;
        particle.vel.y = rand() % 3;
        particle.vel.z = rand() % 3;

        (*particles)[i] = particle;
    }
}

void calculation_slow(Particle **particles)
{
    for (int i = 0; i < N; ++i)
    {
        float_3 netAccel;
        netAccel.x = netAccel.y = netAccel.z = 2;
        for (int j = 0; j < N; ++j)
        {
            float_3 r;
            r.x = (*particles)[j].pos.x - (*particles)[i].pos.x;
            r.y = (*particles)[j].pos.y - (*particles)[i].pos.y;
            r.z = (*particles)[j].pos.z - (*particles)[i].pos.z;

            float dst = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
            float iDst = 1.0f / dst;
            float deltaAccel = iDst * iDst * iDst * 3;
            netAccel.x += r.x * deltaAccel;
            netAccel.y += r.y * deltaAccel;
            netAccel.z += r.z * deltaAccel;
        }
        (*particles)[i].acc.x += netAccel.x;
        (*particles)[i].acc.y += netAccel.y;
        (*particles)[i].acc.z += netAccel.z;
    }
}

void calculation_fast(Particle **particles)
{
    float X[N];
    float Y[N];
    float Z[N];

    for (int i = 0; i < N; i++)
    {
        X[i] = (*particles)[i].pos.x;
        Y[i] = (*particles)[i].pos.y;
        Z[i] = (*particles)[i].pos.z;
    }

    for (int i = 0; i < N; ++i)
    {
        float_3 netAccel;
        netAccel.x = netAccel.y = netAccel.z = 2;
        for (int j = 0; j < N; ++j)
        {
            float_3 r;
            r.x = X[j] - X[i];
            r.y = Y[j] - Y[i];
            r.z = Z[j] - Z[i];

            float dst = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
            float iDst = 1.0f / dst;
            float deltaAccel = iDst * iDst * iDst * 3;
            netAccel.x += r.x * deltaAccel;
            netAccel.y += r.y * deltaAccel;
            netAccel.z += r.z * deltaAccel;
        }
        (*particles)[i].acc.x += netAccel.x;
        (*particles)[i].acc.y += netAccel.y;
        (*particles)[i].acc.z += netAccel.z;
    }

    for (int i = 0; i < N; i++)
    {
        (*particles)[i].pos.x = X[i];
        (*particles)[i].pos.y = Y[i];
        (*particles)[i].pos.z = Z[i];
    }
}

void teardown(Particle **particles)
{
    free(*particles);
}

// static void DoSetup(const benchmark::State &state)
// {
//     allocate(&particles);
//     setup(&particles);
// }

// static void DoTeardown(const benchmark::State &state)
// {
//     teardown(&particles);
// }

// static void particles_slow_benchmark(benchmark::State &state)
// {
//     for (auto _ : state)
//     {
//         calculation_slow(&particles);
//     }
// }

// static void particles_fast_benchmark(benchmark::State &state)
// {
//     for (auto _ : state)
//     {
//         calculation_fast(&particles);
//     }
// }

// BENCHMARK(particles_slow_benchmark)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);
// BENCHMARK(particles_fast_benchmark)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);

// BENCHMARK_MAIN();