
#ifndef _PRE_ASSEMBLER_H_
#define _PRE_ASSEMBLER_H_


#include "constants.h"
#include "macro_data.h"

#include <stdio.h>
#include <string.h>

/* this file contains declerations of functions that uses for the pre assumbler stage.
 * the pre assumbler stage is the first stage of the assumbler that takes the source file and translate from code language to machine
 * language.
 * in this project the main goal of pre assumbler stage is to make a destenation file (.am) that expand all 
 * the macros deffenitions (if exsits) from the source file (.as).
*/


/* this function runs through the lines from the source file and coppy it. in the start of the run the destenation file is empty.
 * each coppy line is getting through the line_analyser function that writes the line on destenation file.
 * in the end of the run the destenation file contains the lines from the source file with expand of the macros deffenition.
 * @param src: pointer to the source file (.as).
 * @param des: pointer to the destenation file (.am).
 * @param list: pointer to the macro list.
*/
void pre_assem_line_runner(FILE *src, FILE *des, ptr_mcr *list);


/* this function writes the line from the source file (.as) to the destenation file (.am) according to the method of macro expand.
 * @param line: string that contain the text line from the source file.
 * @param copy_line: string that contain a copy of the text line from the source file.
 * @param src: pointer to the source file (.as).
 * @param des: pointer to the destenation file (.am).
 * @param list: pointer to the macro list.
*/
void pre_assem_line_analyser(char *line, char *copy_line, FILE *src, FILE *des, ptr_mcr *list);

#endif
