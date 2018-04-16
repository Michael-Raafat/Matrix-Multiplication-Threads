#ifndef FILE_PROCESSING_H_INCLUDED
#define FILE_PROCESSING_H_INCLUDED

#include <stdio.h>

/**
 *  matrix file operations
 **/
void open_matrix_file(FILE** matrixFile, char* fileName);
void get_matrix_line(char** matrixInput, FILE** matrixFile);
void close_matrix_file(FILE** matrixFile);
void restart_matrix_file();



/**
 * result matrix file operations
 **/
void open_result_matrix_file(FILE** resultMatrixFile, char* fileName);
void write_result_matrix_row(char** resultMatrix, FILE** resultMatrixFile);
void close_result_matrix(FILE** resultMatrixFile);

#endif // FILE_PROCESSING_H_INCLUDED
