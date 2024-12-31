

#ifndef _SECOND_PASS_
#define _SECOND_PASS_

#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "line_runner.h"

#include <stdio.h>
#include <string.h>


/* this function gets the current line from line runner function and assumption the first run of the assembler to the current line.
 * @param line: a string that contains the current line to use for strtok function to get text word without white space
 * @param ic: pointer to instruction counter
 * @param instruc_img: pointer to the instruction image array
 * @param table: pointer to the symbol table
 * @param ext_list: pointer to list of referred externals symbols
 * return: if there is an error in the current line returns 1, else returns 0
*/
int second_pass_line_analyser(char *line, int *ic, word **instruc_img, sym_table *table, sym_table *ext_list);



#endif
