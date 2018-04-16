#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "thread_manager.h"
// it holds string containing path of a file.
char fileNameHolder[1000];

typedef enum{ false = 0 , true = 1 } bool ;
/**
 * start our application, formatting files and calculating time taken by each method.
 */
void start(bool argumentsSupplied, char* argv[], int argc);
/**
 * it returns 0 if it's a directory.
 * it returns 1 if it's a file.
 * it returns -1 if it's neither nor.
 */
int isFile(const char* name);
/**
 * check the presence of fileName and add the full path.
 * if not exist copy to holder "none".
 */
void isRelative(char * fileName);
/**
 * add the path and suppling a _1 or _2 to filename
 * if .txt file it makes it _1.txt or _2.txt
 */
void modifyFilePath(char * fileName, int num);



// our main
int main(int argc, char* argv[])
{
    // if user input the 3 filenames.
    if (argc == 4)
    {
        start(true, argv, argc);
    }
    else if (argc == 1)  // if user has no input
    {
        start(false, argv, argc);
    }
    else
    {
        printf("your arguments is not valid in our matmul application.\n");
    }
    return 0;
}

void start(bool argumentsSupplied, char* argv[], int argc)
{
    char first_result_filename[1000];
    char second_result_filename[1000];
    char first_matrix_filename[1000];
    char second_matrix_filename[1000];
    if(argumentsSupplied)
    {
        // check if file exist.
        isRelative(argv[1]);
        // put copy filename from holder to first_matrix_filename
        strcpy(first_matrix_filename, fileNameHolder);
        // check if file exist.
        isRelative(argv[2]);
        // put copy filename from holder to second_matrix_filename
        strcpy(second_matrix_filename, fileNameHolder);
        // formating file in the desired format for first method.
        modifyFilePath(argv[3], 1);
        // put copy filename from holder to first_result_filename
        strcpy(first_result_filename, fileNameHolder);
        // formating file in the desired format for second method.
        modifyFilePath(argv[3], 2);
        // put copy filename from holder to first_result_filename
        strcpy(second_result_filename, fileNameHolder);
    }
    else
    {
        // check if file exist.
        isRelative("a.txt");
        // put copy filename from holder to first_matrix_filename
        strcpy(first_matrix_filename, fileNameHolder);
        // check if file exist.
        isRelative("b.txt");
        // put copy filename from holder to second_matrix_filename
        strcpy(second_matrix_filename, fileNameHolder);
        // formating file in the desired format for first method.
        modifyFilePath("c.txt", 1) ;
        // put copy filename from holder to first_result_filename
        strcpy(first_result_filename, fileNameHolder);
        // formating file in the desired format for second method.
        modifyFilePath("c.txt", 2) ;
        // put copy filename from holder to second_result_filename
        strcpy(second_result_filename, fileNameHolder);

    }
    //check if the file exist or not.
    if (!strcmp(first_matrix_filename,"none") ||
        !strcmp(second_matrix_filename,"none"))
    {
        printf("Error: there a file that is not exist.\n");
        return;
    }
    // set up environment by setting up matrices and reading input files.
    set_up_environment(first_matrix_filename, second_matrix_filename, first_result_filename, second_result_filename);
    // is input files is not valid.
    if (isError())
    {
        return;
    }
    // calculating time of method one.
    clock_t strt1, end1;
    struct timeval stop1, start1;
    printf("First method (thread per row):\n");
    strt1 = clock();
    gettimeofday(&start1, NULL);
    //calling method 1.
    solve_matrix_by_row();
    end1 = clock();
    gettimeofday(&stop1, NULL);
    //printf("Seconds taken(s): %lf\n", (double) (stop1.tv_usec - start1.tv_usec) / 1000000);
    printf("Seconds taken(s): %lf\n", ((double) (end1 - strt1)) / CLOCKS_PER_SEC);
    //printf("Microseconds taken(us): %lu\n", stop1.tv_usec - start1.tv_usec);
    printf("number of threads created: %d.\n\n", getThreadsMethodOne());
    printf("******************************************************\n\n");
    // calculating time of method two.
    clock_t strt, end;
    struct timeval stop, start;
    printf("Second method (thread per element):\n");
    strt = clock();
    gettimeofday(&start, NULL);
    //calling method 2.
    solve_matrix_by_element();
    gettimeofday(&stop, NULL);
    end = clock();
    //printf("Seconds taken(s): %lf\n", (double) (stop.tv_usec - start.tv_usec) / 1000000);
    printf("Seconds taken(s) : %lf\n", ((double) (end - strt)) / CLOCKS_PER_SEC);
    //printf("Microseconds taken(us): %lu\n", stop.tv_usec - start.tv_usec);
    printf("number of threads created: %d.\n", getThreadsMethodTwo());
}



int isFile(const char* name)
{
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }

    if(errno == ENOTDIR)
    {
     return 1;
    }
    return -1;
}


void isRelative(char * fileName)
{
    // if fileName is null so it's none.
    if (fileName == NULL)
    {
        strcpy(fileNameHolder, "none");
        return;
    }
    // get current directory.
    char temp[250];
    getcwd(temp, sizeof(temp));
    int count = 0;
    if (temp != NULL) {
        while(temp[count] != '\0') {
            count++;
        }
        temp[count] = '/';
        count++;
    }
    char currentDir[count + strlen(fileName) + 1];
    // copy current directory to our path.
    strcpy(currentDir, temp);
    // adding fileName.
    while(strlen(fileName)) {
        currentDir[count] = fileName[0];
        fileName++;
        count++;
    }
    currentDir[count] = '\0';
    // check if it's exist.
    if(isFile(currentDir) == 1)
    {
       // copy path to holder.
       strcpy(fileNameHolder,currentDir);
    }
    else
    {
       strcpy(fileNameHolder, "none");
    }
}

// do the same as isRelative function
// adds to it _1 or _2.
void modifyFilePath(char * fileName, int num)
{
    char temp[250];
    getcwd(temp, sizeof(temp));
    int count = 0;
    if (temp != NULL) {
        while(temp[count] != '\0') {
            count++;
        }
        temp[count] = '/';
        count++;
    }
    int flag = 0;
    char currentDir[count + strlen(fileName) + 3];
    strcpy(currentDir, temp);
    while(strlen(fileName)) {
        // to add _1 or _2 before '.'
        if(fileName[0] == '.')
        {
            flag = 1;
            currentDir[count++] = '_';
            currentDir[count++] = '0' + num;
        }
        currentDir[count] = fileName[0];
        fileName++;
        count++;
    }
    // to add _1 or _2 at the end of fileName.
    if (flag == 0 )
    {
        currentDir[count++] = '_';
        currentDir[count++] = '0' + num;
    }
    currentDir[count] = '\0';
    // copy path to holder.
    strcpy(fileNameHolder, currentDir);
}
