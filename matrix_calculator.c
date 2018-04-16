#include "matrix_calculator.h"
#include "thread_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void calculate_matrix_row(void* args)
{
    // cast the parameter.
    struct CalculatorArgs* calculations = (struct CalculatorArgs*) args;
    int length = calculations->first.numberOfColumns;
    int i = 0;
    int row_num = calculations->row;
    // for loop to iterate through second matrix columns to calculate all elements in the row.
    for (i = 0; i < calculations->second.numberOfColumns; i++)
    {
        int j;
        calculations->result->array[row_num][i] = 0;
        // for loop to iterate through elements of row of first matrix
        //and elements of a certain column of second matrix.
        for (j = 0; j < length; j++)
        {
            calculations->result->array[row_num][i] +=
            calculations->first.array[row_num][j] * calculations->second.array[j][i];
        }
    }
}


void calculate_matrix_element(void* args)
{
    // cast the parameter.
    struct CalculatorArgs* calculations = (struct CalculatorArgs*) args;
    int row_num = calculations->row;
    int column_num = calculations->column;
    int length = calculations->first.numberOfColumns;
    int j = 0;
    calculations->result->array[row_num][column_num] = 0;
    // for loop to iterate through elements of a certain row of first matrix
    // and elements of a certain column of second matrix to calculate matrix element.
    for (j = 0; j < length; j++)
    {
        calculations->result->array[row_num][column_num] +=
        calculations->first.array[row_num][j] * calculations->second.array[j][column_num];
    }
}

