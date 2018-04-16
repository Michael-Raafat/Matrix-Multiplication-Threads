#include "thread_manager.h"
#include "file_processing.h"
#include "matrix_parser.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include "matrix_calculator.h"
// files needed.
FILE* firstMatrixFile;
FILE* secondMatrixFile;
FILE* resultMatrixFile;
FILE* resultMatrixSecondFile;
// number of threads created in method 2.
int numberOfthreadsMethod1 = 0;
// number of threads created in method 2.
int numberOfthreadsMethod2 = 0;
// indicates presence of error in matrices inputs.
int error = 0;

/**
 * setup the matrix by filling matrix with file input.
 *
 */
void set_up_matrix(FILE** matrixFile, struct Matrix* matrix);
/**
 * write result into files and free memory taken.
 *
 */
void write_result_and_update(struct CalculatorArgs* calculations[], int threads_num, int file_num);
// matrices structures.
struct Matrix firstMatrix;
struct Matrix secondMatrix;
struct Matrix firstResultMatrix;
struct Matrix secondResultMatrix;

void set_up_environment(char* firstMatrixPath, char* secondMatrixPath, char* resultFirstFilePath, char* resultSecondFilePath)
{
    // open files.
    open_matrix_file(&firstMatrixFile, firstMatrixPath);
    open_matrix_file(&secondMatrixFile, secondMatrixPath);
    open_result_matrix_file(&resultMatrixFile, resultFirstFilePath);
    open_result_matrix_file(&resultMatrixSecondFile, resultSecondFilePath);
    // set up first matrix.
    set_up_matrix(&firstMatrixFile, &firstMatrix);
    if (error)
    {
        return;
    }
    // set up second matrix.
    set_up_matrix(&secondMatrixFile, &secondMatrix);
    if (error)
    {
        return;
    }
    //check for ability to multiply two matrices.
    if (firstMatrix.numberOfColumns != secondMatrix.numberOfRows)
    {
        printf("Error: can't multiply the two matrices ( n*m x m*r = n*r ).\n");
        error = 1;
        return;
    }
    // setting up first result file.
    firstResultMatrix.array = (int **) malloc( firstMatrix.numberOfRows * sizeof(int *));
    int i = 0;
    for (i = 0; i < firstMatrix.numberOfRows; i++)
    {
        firstResultMatrix.array[i] = malloc(secondMatrix.numberOfColumns * sizeof(int *));
    }
    firstResultMatrix.numberOfRows = firstMatrix.numberOfRows;
    firstResultMatrix.numberOfColumns = secondMatrix.numberOfColumns;
    // setting up second result file.
    secondResultMatrix.array = (int **) malloc( firstMatrix.numberOfRows * sizeof(int *));
    for (i = 0; i < firstMatrix.numberOfRows; i++)
    {
        secondResultMatrix.array[i] = malloc(secondMatrix.numberOfColumns * sizeof(int *));
    }
    secondResultMatrix.numberOfRows = firstMatrix.numberOfRows;
    secondResultMatrix.numberOfColumns = secondMatrix.numberOfColumns;
}

void solve_matrix_by_row()
{
    int threads_num = firstMatrix.numberOfRows;
    // allocate a pointer to thread.
    pthread_t *threads = malloc(threads_num * sizeof(pthread_t));
    pthread_t *temp = threads;
    struct CalculatorArgs *calculations[threads_num];
    int i;
    int val;
    int count = 0;
    int k = 0;
    // creating thread per row to calculate result.
    for (k = 0; k < threads_num; k++)
    {
        calculations[count] = malloc(sizeof(struct CalculatorArgs));
        calculations[count]->first = firstMatrix;
        calculations[count]->second = secondMatrix;
        calculations[count]->row = k;
        calculations[count]->result = &firstResultMatrix;
        val = pthread_create(threads, NULL,(void *) calculate_matrix_row, (void *) calculations[count]);
        // if not 0 then failed to create thread.
        if (val)
        {
            printf("ERROR; return code from pthread_create() is %d\n result in this method is not correct.\n", val);
            break;
        }
        threads++;
        count++;
    }
    // join threads.
    for(i = 0; i < count; i++)
    {
        pthread_join(temp[i], NULL);
    }
    // write result and free memory.
    write_result_and_update(calculations, count, 1);
    // setting number of threads created.
    numberOfthreadsMethod1 = count;
}


void solve_matrix_by_element()
{
    int threads_num_row = firstMatrix.numberOfRows;
    int threads_num_column = secondMatrix.numberOfColumns;
    int threads_num = threads_num_column * threads_num_row;
    // allocate a pointer to thread.
    pthread_t *threads = malloc(threads_num * sizeof(pthread_t));
    pthread_t *temp = threads;
    struct CalculatorArgs *calculations[threads_num];
    int i;
    int j;
    int count = 0;
    int val;
    // creating thread per element to calculate result.
    for (i = 0; i < threads_num_row; i++)
    {
        for (j = 0; j < threads_num_column; j++)
        {
            calculations[count] = malloc(sizeof(struct CalculatorArgs));
            calculations[count]->first = firstMatrix;
            calculations[count]->second = secondMatrix;
            calculations[count]->row = i;
            calculations[count]->result = &secondResultMatrix;
            calculations[count]->column = j;
            val = pthread_create(threads, NULL, (void *) calculate_matrix_element, (void *) calculations[count]);
            // if not 0 then failed to create thread.
            if (val)
            {
                printf("ERROR; return code from pthread_create() is %d.\nResult in this method is not correct.\n", val);
                error = 1;
                break;
            }
            threads++;
            count++;
        }
        if (error == 1)
        {
            break;
        }
    }
    // join threads.
    for(i = 0; i < count; i++)
    {
        pthread_join(temp[i], NULL);
    }
     // write result and free memory.
    write_result_and_update(calculations, count, 2);
    // setting number of threads created.
    numberOfthreadsMethod2 = count;
}


void set_up_matrix(FILE** matrixFile, struct Matrix* matrix)
{
    char *matrixInput = malloc(1600000);
    // restart file to begin from first line..
    restart_matrix_file();
    // get first line in file.
    get_matrix_line(&matrixInput, matrixFile);
    // parse it to get number of rows and columns.
    parse_matrix_rows_and_columns(&(*matrix).numberOfColumns, &(*matrix).numberOfRows, &matrixInput);
    (*matrix).array = (int **)malloc((*matrix).numberOfRows * sizeof(int *));
    int firstTime = 1;
    int count = 0;
    // reading lines of matrix and parsing it.
    while ((matrixInput && strlen(matrixInput) > 0 && !error) || firstTime )
    {
        if (count > (*matrix).numberOfRows)
        {
            printf("Error : expected number of rows is %d.\n", (*matrix).numberOfRows);
            error = 1;
        }
        firstTime = 0;
        get_matrix_line(&matrixInput, matrixFile);
        if (strlen(matrixInput) > 0 && !isEmptyLine(matrixInput) && !error)
        {
            parse_matrix(matrix, &matrixInput, count++);
            // if not valid set error = 1;
            if(!isValid())
            {
                error = 1;
            }
        }
    }
}


void write_result_and_update(struct CalculatorArgs* calculations[], int threads_num, int file_num)
{
    int i,j;
    // printing result rows to result file.
    for (i = 0; i < firstMatrix.numberOfRows; i++)
    {
        // collector contains each line in matrix.
        char* collector = malloc(160000);
        strcpy(collector, "\0");
        for(j = 0; j < secondMatrix.numberOfColumns; j++)
        {
            char temp[20];
            // to switch between first and second result matrices.
            if (file_num == 1)
            {
                snprintf(temp, sizeof(temp),"%d\t", firstResultMatrix.array[i][j]);
            }
            else
            {
                snprintf(temp, sizeof(temp),"%d\t", secondResultMatrix.array[i][j]);
            }
            strcat(collector, temp);
        }
        // to switch between first and second result matrices.
        if (file_num == 1)
        {
            write_result_matrix_row(&collector, &resultMatrixFile);
        }
        else
        {
            write_result_matrix_row(&collector, &resultMatrixSecondFile);
        }
        free(collector);
    }
    // free memory used in structures to be passed as arguments.
    for(i = 0; i < threads_num; i++)
    {
        free(calculations[i]);
    }
}

int getThreadsMethodOne()
{
    return numberOfthreadsMethod1;
}

int getThreadsMethodTwo()
{
    return numberOfthreadsMethod2;
}

int isError()
{
    return error;
}
