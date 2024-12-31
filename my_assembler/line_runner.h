

#ifndef _LINE_RUNNER_
#define _LINE_RUNNER_

#include "constants.h"
#include "macro_data.h"
#include "symbol_table.h"
#include "first_pass.h"
#include "second_pass.h"

#include <stdio.h>
#include <string.h>

/* This function prefrom line runner fo first and second pass. 
 * At this stage the assembler runs through the lines from the source file after the expand macros 
 * and process each line from the source file through first_pass_line_analyser function.
 * in the end of the file if there is atleast 1 error the function returns 1 to indicate the error flag of the assembler else returns 0.
 * 
*/
int line_runner(FILE *fp, int *ic, int *dc, word **instruc_img, data_word **data_img, sym_table *table, ptr_mcr m_list, sym_table *ext_list, int is_first);

#endif
