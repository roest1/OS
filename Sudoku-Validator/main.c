#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdatomic.h>

#define N 9

/**
 * Name: Oest, Riley
 * Email: roest1@lsu.edu
 * Project: PA-1 (Multithreading)
 * Instructor: Feng Chen
 * Class: cs4103-sp23
 * Login ID: cs410355
 */

unsigned int grid[N][N]; // copy of solution

// atomic variable eliminate race conditions
atomic_uint count_valid_rows = ATOMIC_VAR_INIT(0);
atomic_uint count_valid_cols = ATOMIC_VAR_INIT(0);
atomic_uint count_valid_subgrids = ATOMIC_VAR_INIT(0);

// struct for passing parameters to threads
typedef struct __params_t
{
    int row;
    int col;
    int t_count;
} params_t;

// thread function for checking a row
void *check_row(void *arg)
{
    params_t *data = (params_t *)arg;
    unsigned int row = data->row;
    unsigned int thread = data->t_count;

    char *result = malloc(50 * sizeof(char));                 // allocating memory for result buffer
    sprintf(result, "[Thread %d] Row %d: ", thread, row + 1); // write to result buffer

    bool valid[N] = {false}; // 9 indicies to check in this row
    for (unsigned int i = 0; i < N; i++)
    {
        unsigned int val = grid[row][i];
        if (val < 1 || val > 9 || valid[val - 1])
        {
            sprintf(result + strlen(result), "Invalid"); // write invalid case to result buffer
            pthread_exit(result);
        }
        else
        {
            valid[val - 1] = true; // val no longer available in row
        }
    }
    free(data);                                // deallocate memory for parameters
    sprintf(result + strlen(result), "Valid"); // write valid case to result buffer
    atomic_fetch_add(&count_valid_rows, 1);    // atomic increment
    pthread_exit(result);
}

// thread function for checking a column
void *check_col(void *arg)
{
    params_t *data = (params_t *)arg;
    unsigned int col = data->col;
    unsigned int thread = data->t_count;

    char *result = malloc(50 * sizeof(char));                    // allocate memory for result buffer
    sprintf(result, "[Thread %d] Column %d: ", thread, col + 1); // write to result buffer

    bool valid[N] = {false}; // 9 indicies to check in this column
    for (unsigned int i = 0; i < N; i++)
    {
        unsigned int val = grid[i][col];
        if (val < 1 || val > 9 || valid[val - 1])
        {
            sprintf(result + strlen(result), "Invalid"); // write invalid case to result buffer
            pthread_exit(result);
        }
        else
        {
            valid[val - 1] = true; // val no longer available in columnn
        }
    }
    free(data);                                // deallocate memory for parameters
    sprintf(result + strlen(result), "Valid"); // write valid case to result buffer
    atomic_fetch_add(&count_valid_cols, 1);    // atomic increment
    pthread_exit(result);
}

// thread function for checking a subgrid
void *check_subgrid(void *arg)
{
    params_t *data = (params_t *)arg;
    unsigned int row = data->row;
    unsigned int col = data->col;
    unsigned int thread = data->t_count;

    char *result = malloc(50 * sizeof(char));                                                                              // allocate memory for the result string
    sprintf(result, "[Thread %d] Subgrid R%d%d%dC%d%d%d: ", thread, row + 1, row + 2, row + 3, col + 1, col + 2, col + 3); // write to result buffer

    bool valid[9] = {false}; // 9 indicies in this 3x3 subgrid to check
    for (unsigned int i = row; i < row + 3; i++)
    {
        for (unsigned int j = col; j < col + 3; j++)
        {
            unsigned int val = grid[i][j];
            if (val < 1 || val > 9 || valid[val - 1])
            {
                sprintf(result + strlen(result), "Invalid"); // write invalid case to result buffer
                pthread_exit(result);
            }
            else
            {
                valid[val - 1] = true; // val no longer available in 3x3 subgrid
            }
        }
    }
    free(data);                                 // deallocate memory for parameters
    sprintf(result + strlen(result), "Valid");  // write valid case to result buffer
    atomic_fetch_add(&count_valid_subgrids, 1); // atomic increment
    pthread_exit(result);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error: must provide input and output files\n");
        return 1;
    }
    FILE *read_file = fopen(argv[1], "r");
    FILE *write_file = fopen(argv[2], "w");
    if (read_file == NULL)
    {
        printf("Error: could not open input file\n");
        return 1;
    }
    if (write_file == NULL)
    {
        printf("Error: could not open output file\n");
        return 1;
    }

    printf("Beginning Validation\n");

    // reading the solution and appending to grid[9][9]
    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int j = 0; j < N; j++)
        {
            fscanf(read_file, "%d", &grid[i][j]);
        }
    }

    pthread_t threads[27];    // create threads for checking each row, column, and subgrid
    unsigned int t_count = 0; // index to track each thread in threads[]

    // 9 threads checking the 9 rows
    for (unsigned int i = 0; i < N; i++)
    {
        params_t *params = (params_t *)malloc(sizeof(params_t));
        params->row = i;
        params->col = 0;
        params->t_count = t_count + 1;
        pthread_create(&threads[t_count++], NULL, check_row, params); // assign worker thread a row
    }
    // 9 threads checking the 9 columns
    for (unsigned int i = 0; i < N; i++)
    {
        params_t *params = (params_t *)malloc(sizeof(params_t));
        params->row = 0;
        params->col = i;
        params->t_count = t_count + 1;
        pthread_create(&threads[t_count++], NULL, check_col, params); // assign worker thread a column
    }
    // 9 threads checking the 9 subgrids
    for (unsigned int i = 0; i < N; i += 3)
    {
        for (unsigned int j = 0; j < N; j += 3)
        {
            params_t *params = (params_t *)malloc(sizeof(params_t));
            params->row = i;
            params->col = j;
            params->t_count = t_count + 1;
            pthread_create(&threads[t_count++], NULL, check_subgrid, params); // assign worker thread a subgrid
        }
    }

    // Wait for all threads to finish
    for (unsigned int i = 0; i < t_count; i++)
    {
        void *result;                                // instantiate buffer
        pthread_join(threads[i], &result);           // append thread's result to buffer
        fprintf(write_file, "%s\n", (char *)result); // write the buffer to the file
        free(result);                                // free memory allocated for the result buffer
    }

    // print results
    fprintf(write_file, "%s%u\n%s%u\n%s%u\n", "Valid rows: ", atomic_load(&count_valid_rows), "Valid columns: ", atomic_load(&count_valid_cols), "Valid subgrids: ", atomic_load(&count_valid_subgrids));
    if (count_valid_rows == 9 && count_valid_cols == 9 && count_valid_subgrids == 9)
    {
        fprintf(write_file, "%s\n", "This Sudoku solution is: Valid");
    }
    else
    {
        fprintf(write_file, "%s\n", "This Sudoku solution is: Invalid");
    }

    // close input and output files
    fclose(read_file);
    fclose(write_file);

    printf("Validation Finished\n");

    return 0;
}