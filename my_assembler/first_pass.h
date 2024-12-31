
#ifndef _FIRST_PASS_
#define _FIRST_PASS_

#include "constants.h"
#include "macro_data.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "line_runner.h"
#include "first_pass_utils.h"
#include "operation.h"

#include <stdio.h>
#include <string.h>


/* this function gets the current line from line runner function and assumption the first run of the assembler to the current line.
 * @param line: the current line input (used in string_sentence function)
 * @param copy_line: the copy of the current line to use for strtok function to get text word without white space
 * @param dc: pointer to data counter
 * @param ic: pointer to instruction counter
 * @param instruc_img: pointer to the instruction image array
 * @param data_img: pointer to the data image array
 * @param table: pointer to the symbol table
 * @param m_list: the macro list for check label deffinition
 * return: if there is an error in the current line returns 1, else returns 0
*/
int first_pass_line_analyser(char *line, char *copy_line, int *ic, int *dc, word **instruc_img, data_word **data_img, sym_table *table, ptr_mcr m_list);



#endif
