#include <stdio.h>
#include <stdlib.h>

#define ROWS 5000
#define COLS 5000

int main(int argc, int argv[])
{
    int **arr;

    arr = calloc(ROWS, sizeof(int *));

    for (int row = 0; row < COLS; row++)
    {
        arr[row] = calloc(COLS, sizeof(int *));
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            arr[i][j] = (i * j) % 123;
        }
    }

    int sum = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            sum += arr[i][j];
        }
    }

    for (int j = 0; j < COLS; j++)
    {
        for (int i = 0; i < ROWS; i++)
        {
            sum += arr[i][j];
        }
    }

    return 0;
}