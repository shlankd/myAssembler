
#include "constants.h"
#include "macro_data.h"
#include "symbol_table.h"
#include "first_pass.h"
#include "second_pass.h"

#include <stdio.h>
#include <string.h>

int line_runner(FILE *fp, int *ic, int *dc, word **instruc_img, data_word **data_img, sym_table *table, ptr_mcr m_list, sym_table *ext_list, int is_first){

	char line[MAX_CHAR_LINE]; /* line text from the source file */
	char copy_line[MAX_CHAR_LINE]; /* the copy line from the source file */
	char c; /* character from the source file */
	int i; /* iterator for char array */
	int curr_line_err_flag;
	int err_flag;

	memset(line, 0, MAX_CHAR_LINE);
	memset(copy_line, 0, MAX_CHAR_LINE);
	i = 0;
	err_flag=0;
	curr_line_err_flag=0;

	while( (c = fgetc(fp)) != EOF ){

	/* assumption that number of characters in line from the source file are correct with maximum 81 characters (include '\n') */
		
		/* Condition for not go out of boundery of the char array src */
		if(i == MAX_CHAR_LINE-1){
			c = '\n';
		}

		line[i] = c;

		/* condition to the end of line from source file */
		if(c == '\n'){

			strncpy(copy_line, line, i);

			/* if the assembler at first run */
			if(is_first){		
				/* then execute first run */	
				curr_line_err_flag = first_pass_line_analyser(line, copy_line, ic, dc, instruc_img, data_img, table, m_list);

			}

			else{
				/* then execute second run */
				curr_line_err_flag = second_pass_line_analyser(copy_line, ic, instruc_img, table, ext_list);
			}

			memset(line, 0, MAX_CHAR_LINE);
			memset(copy_line, 0, MAX_CHAR_LINE);
			i = 0;
		}
		else{
			++i;
		}

		/* condition if the general err_flag is off and in the current line occured an error */
		if(err_flag == 0 && curr_line_err_flag == 1){
			err_flag = curr_line_err_flag; /* then turn on the general err flag */
		}
	}

	/* condition when the loop ended by EOF and read some characters from the last line */
	if(i > 0 && is_first){ /* condition for first pass last line */
		strcpy(copy_line, line);
		curr_line_err_flag = first_pass_line_analyser(line, copy_line, ic, dc, instruc_img, data_img, table, m_list);
	}

	else if(i > 0 && !is_first){ /* condition for second pass last line */
		strcpy(copy_line, line);
		curr_line_err_flag = second_pass_line_analyser(copy_line, ic, instruc_img, table, ext_list);
	}

	/* condition if the general err_flag is off and in the current line occured an error */
	if(err_flag == 0 && curr_line_err_flag == 1){
		err_flag = curr_line_err_flag; /* then turn on the general err flag */
	}

	/* if the assembler at first run */
	if(err_flag == 0 && is_first){
		update_sym_vals(*table, *ic+100, data); /* increament the value of each symbol from the table with  attribute data  */
	}

	return err_flag;
}
