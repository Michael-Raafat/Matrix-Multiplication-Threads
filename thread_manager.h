#ifndef THREAD_MANAGER_H_INCLUDED
#define THREAD_MANAGER_H_INCLUDED

struct Matrix {
    int numberOfRows;
    int numberOfColumns;
    int** array;
};


struct CalculatorArgs {
    int row;
    int column;
    struct Matrix first;
    struct Matrix second;
    struct Matrix *result;
};

/**
 * open all files and getting first and second matrices.
 * setting rows and columns of first and second matrices.
 */
void set_up_environment(char* firstMatrixFile, char* secondMatrixFile, char* resultMatrixFirstFile, char* resultMatrixsecondFile);
/**
 * solve problem by method 1
 * a thread per row.
 */
void solve_matrix_by_row();
/**
 * solve problem by method 2
 * a thread per element.
 */
void solve_matrix_by_element();
/**
 * writing rseult matrix into files.
 * free memory of calclatorArgs used.
 */
void write_result_and_update(struct CalculatorArgs* calculations[], int thread_num, int file_num);
/**
 * get number of threads created in method 1.
 */
int getThreadsMethodOne();
/**
 * get number of threads created in method 2.
 */
int getThreadsMethodTwo();
/**
 * returns 1 if there is an error in the two matrices otherwise return 0.
 */
int isError();
#endif // THREAD_MANAGER_H_INCLUDED
