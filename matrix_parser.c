#include "matrix_parser.h"
#include "thread_manager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
/**
 * convert string into integer
 * it also checks if there is a character that is not a number to check validity of input.
 */
int stringtoInteger(char* number);

// a flag to check if the matrix input is valid or not.
int valid_matrix = 1;

void parse_matrix_rows_and_columns(int* columns, int* rows, char** matrixline)
{
    char* temp = malloc(strlen(* matrixline));
    strcpy(temp, *matrixline);
    strsep(&temp, "=");
    char* row = strsep(&temp, " ");
    *rows = stringtoInteger(row);
    strsep(&temp, "=");
    char* column = strsep(&temp, "\n");
    *columns = stringtoInteger(column);
}

void parse_matrix(struct Matrix* matrix, char** matrixline, int row)
{
    valid_matrix = 1;
    char* ptr = malloc(strlen(*matrixline) + 1);
    strcpy(ptr, *matrixline);
    char* original = ptr;
    // allocate memory for number of elements in each row.
    (*matrix).array[row] = malloc((*matrix).numberOfColumns * sizeof(int *));
    int numberofColumns = (*matrix).numberOfColumns;
    char * rowPointer = strsep(&ptr, "\n");
    strcat(rowPointer, "\0");
    int i = 0;
    // while loop to add elements to matrix.
    // with checks for validity.
    while (i < numberofColumns && valid_matrix)
    {
        char temp[10];
        if (!rowPointer)
        {
            printf("Error: number of columns is %d not %d.\n", numberofColumns, i);
            valid_matrix = 0;
            break;
        }
        strcpy(temp, strsep(&rowPointer, "\t"));
        if (temp != NULL && strlen(temp) > 0)
        {
            (*matrix).array[row][i] = stringtoInteger(temp);
        }
        else
        {
            printf("Error: number of columns is %d not %d.\n", numberofColumns, i);
            valid_matrix = 0;
        }
        i++;
    }
    // while loop to check if there is any extra elements that make the matrix invalid.
    while(rowPointer && (rowPointer[0] == ' ' || rowPointer[0] == '\t') )
    {
        rowPointer++;
    }
    if (rowPointer && strlen(rowPointer) > 0 && (rowPointer[0] != ' ' || rowPointer[0] != '\t'))
    {
        valid_matrix = 0;
        printf("Error : %s is an extra element in row %d, number of columns is %d.\n", rowPointer, row, numberofColumns);
    }
    // free memory located.
    free(original);
}

int isValid()
{
    return valid_matrix;
}

int isEmptyLine(char *input)
{
    int length = strlen(input);
    char temp[length];
    // take a copy from the input line.
    strcpy(temp, input);
    int i = 0;
    // while loop to go through the string to find if it contains a character of not.
    while(i < length && (temp[i] == ' ' || temp[i] == '\t')) {
        i++;
    }
    if (temp[i] == '\n') {
        return 1;
    }
    return 0;
}


int stringtoInteger(char* number)
{
    char* temp = number;
    // while loop to check for any character that is not a digit.
//    while(strlen(temp))
//    {
//        if (!isdigit(temp[0]) && temp[0] != ' ')
//        {
//            valid_matrix = 0;
//            printf("Error: There exist a character in Matrix, Matrix should contain Integers.\n");
//            return -1;
//        }
//        temp++;
//    }
    // atoi() function convert a string to integer.
    return atoi(number);
}
