
#ifndef _FIRST_PASS_UTILS_
#define _FIRST_PASS_UTILS_

#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "functions_with_check.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* this file contains functions that executes the instruction sentences and define sentence 
 * for first pass stage of the assembler. In each function, if discovered that there is a syntax error the function prints an error
 * message and returns 1 to turn on the error flag of the assembler, else the function returns 0 and won't turn on the error flag.
*/


/* this function gets the lable name parameter in the external sentence and add it to the symbol table with the attribute external.
 * if there is an error in the lable name then the function prints an error message and returns 1 to turn on the error flag.
 * @param lable_name: a string that contains the lable name in the external sentence
 * @param num_of_line: the current line number of the first pass. use for error message to indicate in which line occure an error
 * @param table: pointer to the symbol table
 * return: if there is an error  returns 1, else returns 0
*/
int external_sentence(char *lable_name, int num_of_line, sym_table *table);


/* this function take the ascii code from each character parameter that in the quotation mark and insert it into the array of data image. 
 * @param str_deff: a token string that contains each sequence of chars from the current line in the first pass
 *		     the str_deff is sequence of chars after the instruction ".string" from the source file and contains chars parameters
 * @param num_of_line: the current line number of the first pass. use for error message to indicate in which line occure an error
 * @param dc: pointer to data counter
 * @param data_img: pointer to the data image array
 * @param line: the current line from the input file
 * return: if there is syntax error  returns 1, else returns 0
*/
int string_sentence(char *line, char *str_deff, int num_of_line, int *dc, data_word **data_img);


/* this function take each number parameter and insert it into the array of data image 
 * @param data_vals: a token string that contains each sequence of chars from the current line in the first pass
 *		     the data_vals is sequence of chars after the instruction ".data" from the source file and contains list of numbers
 * @param num_of_line: the current line number of the first pass. use for error message to indicate in which line occure an error
 * @param dc: pointer to data counter
 * @param data_img: pointer to the data image array
 * @param table: pointer to the symbol table
 * return: if there is syntax error  returns 1, else returns 0
*/
int data_sentence(char *data_vals, int num_of_line, int *dc, data_word **data_img, sym_table *table);


/* this function gets the value and the lable name of the defined constant and add is to the symbol table with mdefine attribute.
 * @param deff_const: a token string that contains each sequence of chars from the current line in the first pass
 *		     deff_const is sequence of chars after the ".define" from the source file and contains integer value for the constant
 * @param num_of_line: the current line number of the first pass. use for error message to indicate in which line occure an error
 * @param dc: pointer to data counter
 * @param table: pointer to the symbol table
 * return: if there is syntax error  returns 1, else returns 0
*/
int define_sentence(char *deff_const, int num_of_line, sym_table *table);

/* this function checks the valid of a single data parameter. 
 * if the data parameter is valid then the function apply the value of the data parameter in the data image and returns 1.
 * if the data parameter is not valid then the function prints an error and returns 0.
 * @param val_str: data parameter as a string
 * @param num_of_line: the current line number of the first pass. use for error message to indicate in which line occure an error
 * @param dc: pointer to data counter
 * @param data_img: pointer to the data image array
 * @param table: pointer to the symbol table
 * return: if the data parameter is valid returns  else returns 0;
*/
int is_data_applied(char *val_str, int num_of_line, int *dc, data_word **data_img, sym_table *table);


#endif
