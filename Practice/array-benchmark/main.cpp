#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <stdbool.h>
#include <omp.h>

#include <tracy/Tracy.hpp>
#include <benchmark/benchmark.h>

#define ROWS 5000
#define COLS 5000

#define N (1 << 25)

int** arr;

void setup();
void teardown();
void rows_cols();
void cols_rows();
int rows_cols_sum();
int cols_rows_sum();
void matrix_multiplications();
void loop_dependence();
void loop_unrolling_slow(double* array, int n, double result);
void loop_unrolling_fast(double* array, int n, double result);
void cache_associativity_limit(int step);
void prefetching();
void hot_cold();
void aos_soa();
void struct_size();
double array_sum(double* array);
void array_add(double* A, double* B, double* results);
void array_add_parallel(double* A, double* B, double* results);
void predictability();
void true_sharing();
void false_sharing();
void false_sharing_fix();

double vector_add_simd()
{
	ZoneScopedS(5);

	// Vector
	double* array = (double*)_aligned_malloc(sizeof(double) * N, 16);

	// SSE specific step
	int step = 128 / (sizeof(double) * 8);
	int sumSize = step;
	double* sum = (double*)malloc(sizeof(double) * sumSize);

	for (int i = 0; i < N; ++i)
	{
		array[i] = 1.0;
	}

	for (int i = 0; i < sumSize; ++i)
	{
		sum[i] = 0.0;
	}

	__m128d sumVector = _mm_loadu_pd(sum);
	__m128d* arrayVector = (__m128d*)array;
	int vecIterations = N / step;

	for (int i = 0; i < vecIterations; i++)
	{
		sumVector = _mm_add_pd(arrayVector[i], sumVector);
	}

	_mm_store_pd(sum, sumVector);

	double sumTotal = 0.0;

	for (int i = 0; i < sumSize; i++)
	{
		sumTotal += sum[i];
	}

	// free(array);
	free(sum);

	return sumTotal;
}

double vector_add()
{
	ZoneScopedS(5);

	// Scalar
	double* array = (double*)malloc(sizeof(double) * N);
	double sum = 0.0;

	// Array initialization

	for (int i = 0; i < N; ++i)
	{
		array[i] = 1.0;
	}

	for (int i = 0; i < N; i++)
	{
		sum += array[i];
	}

	return sum;
}

int8_t* add_vectors()
{
	ZoneScopedS(5);

	int8_t* A = (int8_t*)_aligned_malloc(sizeof(int8_t) * N, 16);
	int8_t* B = (int8_t*)_aligned_malloc(sizeof(int8_t) * N, 16);
	int8_t* C = (int8_t*)_aligned_malloc(sizeof(int8_t) * N, 16);
	int step = 128 / (sizeof(int8_t) * 8);

	for (int i = 0; i < N; ++i)
	{
		A[i] = 1;
		B[i] = 1;
	}

	__m128i* AVector = (__m128i*)A;
	__m128i* BVector = (__m128i*)B;
	__m128i* CVector = (__m128i*)C;

	for (int i = 0; i < step; i++)
	{
		CVector[i] = _mm_add_epi8(AVector[i], BVector[i]);
	}

	return C;
}

int8_t* add_scalars()
{
	ZoneScopedS(5);

	int8_t* A = (int8_t*)malloc(sizeof(int8_t) * N);
	int8_t* B = (int8_t*)malloc(sizeof(int8_t) * N);
	int8_t* C = (int8_t*)malloc(sizeof(int8_t) * N);

	for (int i = 0; i < N; ++i)
	{
		A[i] = 1;
		B[i] = 1;
	}

	for (int i = 0; i < N; i++)
	{
		C[i] = A[i] + B[i];
	}

	return C;
}

int main(void)
{
	ZoneScopedS(5);

	/*int8_t* resScalar = add_scalars();
	printf("%d\n", resScalar[0]);
	free(resScalar);

	int8_t* resVector = add_vectors();
	printf("%d\n", resVector[0]);*/
	// free(resVector);

	// double res = vector_add();
	// printf("%f\n", res);

	// res = vector_add_simd();
	// printf("%f\n", res);

	// struct_size();

	// double *A = (double *)malloc(sizeof(double) * N);
	// double *B = (double *)malloc(sizeof(double) * N);
	// double *results = (double *)malloc(sizeof(double) * N);

	// for (int i = 0; i < N; i++)
	// {
	//     A[i] = 1.0;
	//     B[i] = 1.0;
	// }

	// array_add(A, B, results);
	// printf("%f", results[0]);

	// array_add_parallel(A, B, results);
	// printf("%f", results[0]);

	true_sharing();

	false_sharing();

	false_sharing_fix();

	// setup();

	// int sum = cols_rows_sum();

	// printf("%d\n", sum);

	// sum = rows_cols_sum();

	// printf("%d\n", sum);

	// teardown();

	return 0;
}

void setup()
{
	ZoneScopedS(5);
	arr = (int**)calloc(ROWS, sizeof(int*));
	TracyAlloc(arr, sizeof(int*));

	for (int row = 0; row < COLS; row++)
	{
		arr[row] = (int*)calloc(COLS, sizeof(int*));
		TracyAlloc(arr[row], sizeof(int*));
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			arr[i][j] = ((i + 1) * (j + 1)) % 123;
		}
	}
}
void teardown()
{
	ZoneScopedS(5);
	for (int row = 0; row < COLS; row++)
	{
		TracyFree(arr[row]);
		free(arr[row]);
	}

	free(arr);
}
int rows_cols_sum()
{
	ZoneScopedS(5);
	int sum = 0;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			sum += arr[i][j];
		}
	}

	return sum;
}
int cols_rows_sum()
{
	ZoneScopedS(5);
	int sum = 0;
	for (int j = 0; j < COLS; j++)
	{
		for (int i = 0; i < ROWS; i++)
		{
			sum += arr[i][j];
		}
	}

	return sum;
}

void matrix_multiplications()
{

	double** A;
	double** B;
	double** C;

	double sum = 0.0;
	int n = 10;

	// Version 1
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

	// Version 2
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

	// Version 3
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

	// Version 4
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

	// Version 5
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

	// Version 6
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

void loop_dependence()
{
	double* A;
	double* B;
	double* C;
	double* D;

	int n = 10; // A number big enough to show the impact

	// With dependence
	for (int i = 0; i < n; i++)
	{
		A[i] = A[i] + B[i];
		B[i + 1] = C[i] + D[i];
	}

	// Without dependence
	A[0] = A[0] + B[0];
	for (int i = 0; i < n - 1; i++)
	{
		B[i + 1] = C[i] + D[i];
		A[i + 1] = A[i + 1] + B[i + 1];
	}
	B[n] = C[n - 1] + D[n - 1];
}

void loop_unrolling_slow(double* array, int n, double* result)
{

	for (int i = 0; i < n; i++)
	{
		*result += array[i];
	}
}

void loop_unrolling_fast(double* array, int n, double* result)
{

	int i = 0;
	int limit = n - 1;
	double accumulator = 0.0;

	// Unrolled loop
	for (; i < limit; i += 2)
	{
		accumulator += array[i];
		accumulator += array[i + 1];
	}

	// Finalizing loop
	for (; i < n; i++)
	{
		accumulator += array[i];
	}

	*result = accumulator;
}

// Steps: 30, 32, 60, 64, 120, 128, 250, 256, 510, 512
void cache_associativity_limit(int step)
{

	double* array;
	int n = 0; // A number to ensure the same number of memory accesses for every step

	// Array initialization

	double sum = 0.0;

	for (int i = 0; i < n; i += step)
	{
		sum += array[i];
	}
}

void prefetching()
{

	double* array;
	int n = 0; // A number to ensure the same number of memory accesses for every step

	// Array initialization

	double sum = 0.0;

	// Sequential
	for (int i = 0; i < n; i++)
	{
		sum += array[i];
	}

	// Strided
	int step;
	for (int i = 0; i < n; i += step)
	{
		sum += array[i];
	}

	// Pattern
	int x;
	for (int i = 0; i < n; i = 2 * x + 5)
	{
		sum += array[i];
	}

	// Pattern witch prefetching
	for (int i = 0; i < n; i = 2 * x + 5)
	{
		//__builtin_prefetch(&array[2 * x + 5]);
		sum += array[i];
	}

	// Random
	int seed;
	srand(seed);

	for (int i = 0; i < n; i++)
	{
		int index = rand() % n;
		sum += array[index];
	}
}

void hot_cold()
{

	// // Original
	// struct Data {
	//     double a, b, c, d, e, f, g, h, result;
	// };

	// Data* data;
	// int n = 0;// Number of elements in data array

	// // Array initialization

	// for (int i = 0; i < n; i++)
	// {
	//     data[i].result = data[i].a + data[i].b * data[i].c;
	// }

	// Hot and cold data separated
	struct DataHot
	{
		double a, b, c, result;
	};

	struct DataCold
	{
		double d, e, f, g, h;
	};

	struct Data
	{
		DataHot* hot;
		DataCold* cold;
	};

	Data* data;
	int n = 0; // Number of elements in data array

	// Array initialization

	for (int i = 0; i < n; i++)
	{
		data[i].hot->result = data[i].hot->a + data[i].hot->b * data[i].hot->c;
	}
}

void aos_soa()
{

	// Array of structures
	// struct AoS {
	//     double a, b, c, d, e, f, g, h, result;
	// };

	// AoS* data;
	// int n = 0;// Number of elements in data array

	// // Array initialization

	// for (int i = 0; i < n; i++)
	// {
	//     data[i].result = data[i].a + data[i].b * data[i].c;
	// }

	// Structure of arrays
	struct SoA
	{
		double* a;
		double* b;
		double* c;
		double* d;
		double* e;
		double* f;
		double* g;
		double* h;
		double* results;
	};

	SoA data;
	int n = 0; // Number of elements in each array of data

	// Array initialization

	for (int i = 0; i < n; i++)
	{
		data.results[i] = data.a[i] + data.b[i] * data.c[i];
	}
}

void struct_size()
{

	struct Data
	{
		bool a;
		int32_t b;
		bool c;
		int64_t d;
		bool e;
	};

	sizeof(Data);

	struct DataOrdered
	{
		int64_t a;
		int32_t b;
		bool c, d, e;
	};

	sizeof(DataOrdered);

	printf("Data: %d, Smaller: %d\n", sizeof(Data), sizeof(DataOrdered));
}

double array_sum(double* array) {

	ZoneScopedS(5);

	double sum = 0.0;

#pragma omp parallel for
	for (int i = 0; i < N; i++)
	{
		sum += array[i];
	}

	return sum;
}

void array_add(double* A, double* B, double* results) {

	ZoneScopedS(5);

	for (int i = 0; i < N; i++)
	{
		results[i] = A[i] + B[i];
	}
}

void array_add_parallel(double* A, double* B, double* results) {

	ZoneScopedS(5);

#pragma omp parallel for
	for (int i = 0; i < N; i++)
	{
		results[i] = A[i] + B[i];
	}
}

void predictability() {

	double* array = (double*)malloc(sizeof(double) * N);
	srand(0);

	// Initialize
	for (int i = 0; i < N; ++i)
	{
		array[i] = rand() % 10;
	}

	// Process predictable
	for (int i = 0; i < N; ++i)
	{
		if (array[i] > 20.0) {
			printf("%f", array[i]);
		}
	}

	// Process predictable
	for (int i = 0; i < N; ++i)
	{
		if (array[i] >= 0.0 && array[i] < 10.0) {
			printf("%f", array[i]);
		}
	}

	// Process not so predictable
	for (int i = 0; i < N; ++i)
	{
		if (array[i] > 5.0) {
			printf("%f", array[i]);
		}
	}

	free(array);
}

void true_sharing() {
	// True sharing
	int shared = 0;

	#pragma omp parallel num_threads(4)
	{
		// Race!!!
		shared++;
	}

	printf("%d\n", shared);
}

void false_sharing() {
	// False sharing
	struct Data {
		int32_t a, b, c, d;
	};

	Data data;
	data.a = 0;
	data.b = 0;
	data.c = 0;
	data.d = 0;

#pragma omp parallel num_threads(4)
	{
		switch (omp_get_thread_num())
		{
		case 0:
			++data.a;
		case 1:
			++data.b;
		case 2:
			++data.c;
		case 3:
			++data.d;
		}
	}
}

#define ALIGNMENT (64)

void false_sharing_fix() {
	// False sharing fix
	struct Data {
		alignas(ALIGNMENT) int32_t a;
		alignas(ALIGNMENT) int32_t b;
		alignas(ALIGNMENT) int32_t c;
		alignas(ALIGNMENT) int32_t d;
	};

	Data data;
	data.a = 0;
	data.b = 0;
	data.c = 0;
	data.d = 0;

#pragma omp parallel num_threads(4)
	{
		switch (omp_get_thread_num())
		{
		case 0:
			++data.a;
		case 1:
			++data.b;
		case 2:
			++data.c;
		case 3:
			++data.d;
		}
	}
}

// static void DoSetup(const benchmark::State &state)
//{
//     arr = (int **)calloc(ROWS, sizeof(int *));

//    for (int row = 0; row < COLS; row++)
//    {
//        arr[row] = (int *)calloc(COLS, sizeof(int *));
//    }

//    for (int i = 0; i < ROWS; i++)
//    {
//        for (int j = 0; j < COLS; j++)
//        {
//            arr[i][j] = ((i + 1) * (j + 1)) % 123;
//        }
//    }
//}

// static void DoTeardown(const benchmark::State &state)
//{
//     for (int row = 0; row < COLS; row++)
//     {
//         free(arr[row]);
//     }

//    free(arr);
//}

// static void rows_cols(benchmark::State &state)
//{
//     for (auto _ : state)
//     {
//         int sum = 0;
//         for (int i = 0; i < ROWS; i++)
//         {
//             for (int j = 0; j < COLS; j++)
//             {
//                 sum += arr[i][j];
//             }
//         }
//     }
// }

// static void cols_rows(benchmark::State &state)
//{
//     for (auto _ : state)
//     {
//         int sum = 0;
//         for (int j = 0; j < COLS; j++)
//         {
//             for (int i = 0; i < ROWS; i++)
//             {
//                 sum += arr[i][j];
//             }
//         }
//     }
// }

// BENCHMARK(rows_cols)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);
// BENCHMARK(cols_rows)->Threads(1)->Threads(8)->Setup(DoSetup)->Teardown(DoTeardown);

// BENCHMARK_MAIN();