#ifndef MATRIX_PARSER_H_INCLUDED
#define MATRIX_PARSER_H_INCLUDED
#include "thread_manager.h"

/**
 * parse first line in the file which contains number of rows and columns.
 */
void parse_matrix_rows_and_columns(int* columns, int* rows, char** matrixline);

/**
 * parse matrix lines.
 * @matrixline is string contains a row in the matrix.
 * @row is the number of row in matrix.
 */
void parse_matrix(struct Matrix* matrix, char** matrixline, int row);
/**
 * it returns 0 if not valid and 1 if is valid.
 */
int isValid();
/**
 * returns 1 if line is empty otherwise returns 0.
 */
int isEmptyLine(char *input);
#endif // MATRIX_PARSER_H_INCLUDED
