#include "file_processing.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// integer represent the place from which we read in the file.
int matrixRowBytes;

/**
 *  MATRIX OPERATIONS
 */
void open_matrix_file(FILE** matrixFile, char* fileName)
{
    // open a file with mode "r" to read the file.
    *matrixFile = fopen(fileName, "r");
}

void get_matrix_line(char** matrixInput, FILE** matrixFile)
{
    // declare a string to store the content of the line in this file.
    char row[160000];
	fseek(*matrixFile, matrixRowBytes, SEEK_SET);
    if (fgets(row, sizeof(row), *matrixFile) == NULL) {
        *matrixInput = "\0";
        return;
    }
    // add the length of row to matrixRowBytes to update the arrow we read from in the file.
    matrixRowBytes += strlen(row);
    //copy the line to the pointer matrixInput to use it in the calling function.
    strcpy(*matrixInput, row);
}

void close_matrix_file(FILE** matrixFile)
{
    // to close the file.
    fclose(*matrixFile);
}


void restart_matrix_file()
{
    // to restart the arrow from which we read the file we set matrixRowBytes to zero
    // in order to begin from the first line.
    matrixRowBytes = 0;
}

/**
 * RESULT MATRIX OPERATIONS
 */
void open_result_matrix_file(FILE** resultMatrixFile, char* fileName)
{
    // open a file with mode "a+" to append lines to this file.
    *resultMatrixFile = fopen(fileName, "a+");
}


void write_result_matrix_row(char** resultMatrix, FILE** resultMatrixFile)
{
    // print the row of the matrix into the file .
    fprintf(*resultMatrixFile, "%s\n", *resultMatrix);
    fflush(*resultMatrixFile);
}

void close_result_matrix(FILE** resultMatrixFile)
{
    // to close the file.
    fclose(*resultMatrixFile);
}


