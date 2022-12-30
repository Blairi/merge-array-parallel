#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define PATH_SERIAL "/home/blairi/development/C/merge-array-parallel/data/serial_time.txt"
#define PATH_PARALLEL "/home/blairi/development/C/merge-array-parallel/data/parallel_time.txt"

void print_array(int arr[], int n);
int * generate_array(int len);
void func();
int * merge_array_serial(int arr1[], int n, int arr2[], int m);
int * merge_array_parallel(int arr1[], int n, int arr2[], int m);
void test_simple();
void test_MAX();


#define MAX 5000


int main(int argc, char const *argv[])
{
    func();
	test_simple();
	printf("\n");
	test_MAX();
    return 0;
}


int * merge_array_serial(int arr1[], int n, int arr2[], int m)
{
	int match_count = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (arr1[i] == arr2[j]) match_count ++;

	int matches[match_count][2];
	int aux = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (arr1[i] == arr2[j])
			{
				matches[aux][0] = i;
				matches[aux][1] = j;
				aux ++;
			}

	int len = n + m - (match_count * 2);
	int *array_merged = malloc( sizeof(int) * len );
	int index = 0;

	for (int i = 0; i < n; i++)
	{
		int flag = -1;
		for (int j = 0; j < match_count; j++)
		{
			if (i == matches[j][0])
			{
				flag = 0;
			}
		}
		if (flag == -1)
		{
			array_merged[index] = arr1[i];
			index = index + 1;
		}
		
	}

	for (int i = 0; i < m; i++)
	{
		int flag = -1;
		for (int j = 0; j < match_count; j++)
		{
			if (i == matches[j][1])
			{
				flag = 0;
			}
		}
		if (flag == -1)
		{
			array_merged[index] = arr2[i];
			index = index + 1;
		}
	}

	return array_merged;
}


int * merge_array_parallel(int arr1[], int n, int arr2[], int m)
{
	int match_count = 0;
	#pragma omp parallel for collapse(2) reduction(+ : match_count)
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (arr1[i] == arr2[j]) match_count ++;

	int matches[match_count][2];
	int aux = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (arr1[i] == arr2[j])
			{
				matches[aux][0] = i;
				matches[aux][1] = j;
				aux ++;
			}

	int len = n + m - (match_count * 2);
	int *array_merged = malloc( sizeof(int) * len );
	int index = 0;

	for (int i = 0; i < n; i++)
	{
		int flag = -1;
		for (int j = 0; j < match_count; j++)
		{
			if (i == matches[j][0])
			{
				flag = 0;
			}
		}
		if (flag == -1)
		{
			array_merged[index] = arr1[i];
			index = index + 1;
		}
	}

	for (int i = 0; i < m; i++)
	{
		int flag = -1;
		for (int j = 0; j < match_count; j++)
		{
			if (i == matches[j][1])
			{
				flag = 0;
			}
		}
		if (flag == -1)
		{
			array_merged[index] = arr2[i];
			index = index + 1;
		}
	}

	return array_merged;
}


void test_simple()
{
	printf("Simple test:\n");
	int arr1[6] = {1, 2, 3, 9, 10, 8};
	int arr2[6] = {4, 6, 7, 10, 5, 1};

	printf("Arr1: ");
	print_array(arr1, 6);
	printf("Arr2: ");
	print_array(arr2, 6);

	float ini, fin;
	ini = omp_get_wtime();
	int *merged = merge_array_serial(arr1, 6, arr2, 6);
	fin = omp_get_wtime();
	printf("Serial merged: ");
	print_array(merged, 8);
	printf("time serial: %f\n", fin - ini);

	ini = omp_get_wtime();
	merged = merge_array_parallel(arr1, 6, arr2, 6);
	fin = omp_get_wtime();
	printf("Parallel merged: ");
	print_array(merged, 8);
	printf("time parallel: %f\n", fin - ini);

}


void test_MAX()
{

	int *arr1 = generate_array(MAX);
	int *arr2 = generate_array(MAX);

	// printf("Arr1: ");
	// print_array(arr1, MAX);
	// printf("Arr2: ");
	// print_array(arr2, MAX);
	printf("Test with %d elements\n", MAX);
	float ini, fin;
	ini = omp_get_wtime();
	int *merged = merge_array_serial(arr1, MAX, arr2, MAX);
	fin = omp_get_wtime();
	printf("Serial merged: ");
	print_array(merged, 0);
	printf("time serial: %f\n", fin - ini);

	ini = omp_get_wtime();
	merged = merge_array_parallel(arr1, MAX, arr2, MAX);
	fin = omp_get_wtime();
	printf("Parallel merged: ");
	print_array(merged, 0);
	printf("time parallel: %f\n", fin - ini);

}


void print_array(int arr[], int n)
{
	int i;
	printf("[ ");
	for (i = 0; i < n; i++) printf("%d ", arr[i]);
	printf("]\n");
}


int * generate_array(int len)
{
	int *arr = malloc( sizeof(int) * len );

	for (int i = 0; i < len; i++)
		arr[i] = i + 1;

	return arr;
}


void func()
{
	FILE *file_serial;
	file_serial = fopen("/home/blairi/development/C/merge-array-parallel/data/serial_time.txt", "w");
	FILE *file_parallel;
	file_parallel = fopen("/home/blairi/development/C/merge-array-parallel/data/parallel_time.txt", "w");

	if ( file_serial == NULL || file_parallel == NULL )
	{
		printf("Error\n");
		exit(1);
	}

	// Resetting files
	fclose(fopen("/home/blairi/development/C/merge-array-parallel/data/serial_time.txt", "w"));
	fclose(fopen("/home/blairi/development/C/merge-array-parallel/data/parallel_time.txt", "w"));

	file_serial = fopen("/home/blairi/development/C/merge-array-parallel/data/serial_time.txt", "a");
	file_parallel = fopen("/home/blairi/development/C/merge-array-parallel/data/parallel_time.txt", "a");

	float ini, fin;
	for (int i = 1; i <= MAX; i++)
	{
		int *arr1 = generate_array( i );
		int *arr2 = generate_array( i );
		ini = omp_get_wtime();
		merge_array_serial(arr1, i, arr2, i);
		fin = omp_get_wtime();

		fprintf(file_serial, "%d:%f\n", i, fin - ini);
	}
	printf("Serial file finished\n");
	fclose(file_serial);

	for (int i = 1; i <= MAX; i++)
	{
		int *arr1 = generate_array( i );
		int *arr2 = generate_array( i );
		ini = omp_get_wtime();
		merge_array_parallel(arr1, i, arr2, i);
		fin = omp_get_wtime();

		fprintf(file_parallel, "%d:%f\n", i, fin - ini);
	}
	printf("Parallel file finished\n");
	fclose(file_parallel);
}
