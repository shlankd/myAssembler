
#ifndef _FUNCTIONS_WITH_CHECK_H_
#define _FUNCTIONS_WITH_CHECK_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

/* this file contain declerations of functions that checks the result of the functions from the libery */

/* this function use the malloc function and check if the memory allocation failed.
 * if so prints a message of failure and exit the program
 * @param size: the size of memory to allocate in bytes
 * @return: a generic pointer to the memory that the malloc successfuly allocated 
*/
void *malloc_and_check(long size);

/* this function uses the fopen function and checks if the the file opened successfully.
 * if the file did not open then the function print to the user error message and returns NULL,
 * else returns a file stream.
 * @param file_name: text of the file name
 * @return: a file stream or NULL
*/
FILE *fopen_and_check(const char *file_name, const char *mode_operation);

/* this function checks if the argument of the input number can be contained with 12 bits in a word.
 * if the input number is valid the function sets the integer from the argument and returns 0 for no error 
 * else returns 1 to indicate an error.
 * @param num_to_set: pointer to the integer from the argument that need to be set
 * @para num_args:  the integer argument that need to check
 * return: if the input integer is invalid returns 1 else returns 0 and sets the pointer integer to point the input number. 
*/
/*int input_num_check_and_set(int *num_to_set, char *num_args);*/


/* this function sets the lable name string and the index string from the operand argument with address type fixed index (type 2)
 * @param label: a string to set with the lable name of the operand
 * @param ind: a string to set with the index of the operand
 * @param operand_arg: the operand argument from the source file with address type 2 (fixed index)
 * return: return 1 if the function failed to set the 2 strings else returns 0
*/
int is_label_and_index_set(char **label_name, char **ind, char *operand_arg);

/* this function checks if input number for immediate can be contained in a word with 12 bits if not the function prints an error.
 * @param check_num: the input number of the immediate to check
 * @param num_line: the line number for print the error location if occour
 * return: if the input number is valid returns 0 for no error else returns 1 to indicate an error.
*/
int is_imd_overflow(int check_num, int num_line);

/* this function checks if input number data can be contained in a word with 14 bits if not the function prints an error.
 * @param check_num: the input number data to check
 * @param num_line: the line number for print the error location if occour
 * return: if the input number is valid returns 0 for no error else returns 1 to indicate an error.
*/
int is_data_overflow(int check_num, int num_line);


/* this function free the instruction image
 * @param instruct_img: the instruction image to free
 * @para size: the size of the instruction image 
*/
void free_instruction_image(word **instruct_img, int size);

/* this function free the data image
 * @param data_img: the data image to free
 * @para size: the size of the data image 
*/
void free_data_image(data_word **data_img, int size);


/* ----------------------------------TO DELETE---------------------------------- */
void print_file(FILE *fp);
/* ----------------------------------TO DELETE---------------------------------- */



#endif

