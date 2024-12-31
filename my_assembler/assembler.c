
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "symbol_table.h"
#include "functions_with_check.h"
#include "pre_assembler.h"
#include "line_runner.h"
#include "constants.h"
#include "files_output.h"

/* this file implements the assumbler functions on imaginary computer */
int file_assembler(char *arg_file_name);

int main(int argc, char *argv[]){
	
	int i;
	int err_flag; /* an error flag to indicate if there is any error in the assembler */
	err_flag = 0;
	for(i=1; i<argc; i++){

	/* assumption: if the assembler found out an error to one of the input files then the assembler won't continue to the next file */
		err_flag = file_assembler(argv[i]);

		if(err_flag){
			printf("the assembler  ended with an error");
			break;
		}
	}

	return 0;
}

int file_assembler(char *arg_file_name){

	FILE *src; /* file pointer to the source file */
	FILE *output; /* file pointer to the output file */	
	char *file_name; /* contain the file name text */
	int i_size; /* contains the size of the image instruction from the input file */

	ptr_mcr macro_list; /* pointer to the macro list */


	int ic,dc;
	data_word *data_img[MAX_IMG_SIZE];
	word *instruction_img[MAX_IMG_SIZE];
	sym_table symbol_table;
	int err_flag;

	sym_table ext_list; /* list of externals symbols that referred at certain operand in the source file */
			    /* the value of each referred symbol external in the list is the address of the code machine */ 

	/* make change to the file name so that at the end of the file name will be with .as */
	file_name = (char*)malloc_and_check(strlen(arg_file_name)+4);
	strcpy(file_name, arg_file_name);
	strcat(file_name, ".as");

	src = fopen_and_check(file_name, "r");
	
	/* condition if the source file is not open */
	if(src == NULL){
		free(file_name);
		return 1; /* return the error flag of the main true value to indicate an error */
	}

	/*--------------------------------start of the pre assembler stage--------------------------------*/

	file_name[strlen(file_name)-1] = 'm'; /* name of the output file of the pre assembler */

	output = fopen_and_check(file_name, "w+"); /* create the output file for the pre assembler */

	/* condition if the file for the pre assembler is not open */
	if(output == NULL){
		free(file_name);
		fclose(src);
		return 1; /* return the error flag of the main true value to indicate an error */
	}

	macro_list = NULL; /* initiate the macro list */

	pre_assem_line_runner(src, output, &macro_list);
	print_list_macro(macro_list);

	/*--------------------------------end of the pre assembler stage--------------------------------*/


	
	/*--------------------------------start of the first pass stage--------------------------------*/
	
	/* initiate the first pass image arrays, counters and symbol table */
	ic = 0;
	dc = 0;
	symbol_table = NULL;
	ext_list = NULL;
	err_flag = 0;
	rewind(output);
	
	/* execute first pass */ 									     /* 1 for execute first pass */
	err_flag = line_runner(output, &ic, &dc, instruction_img, data_img, &symbol_table, macro_list, &ext_list, 1);

	/*--------------------------------end of the first pass stage--------------------------------*/
	



	/*--------------------------------start of the second pass stage--------------------------------*/

	i_size = ic; /* save the amount of instructions words from the input file */
	ic = 0;
	rewind(output);

	/* execute second pass */
	if(!err_flag){											     /* 0 for execute second pass */
		err_flag = line_runner(output, &ic, &dc, instruction_img, data_img, &symbol_table, macro_list, &ext_list, 0); 
	}
	

	/*--------------------------------end of the second pass stage--------------------------------*/



	/*--------------------------------start of the output pass stage--------------------------------*/
	if(!err_flag){
		/* creates output files */
		err_flag = output_files(file_name, i_size, dc, instruction_img, data_img, symbol_table, ext_list); 
	}
	/*--------------------------------end of the output stage--------------------------------*/
	

	/*--------------------------------CCCCCCCCCLLLLLLLLLLLLLOOOOOOOOOOOOSSSESSSSSSEEEEEEEE--------------------------------*/
	free_macro_list(&macro_list);
	free_instruction_image(instruction_img,i_size);
	free_data_image(data_img,dc);
	free_table(&symbol_table);
	free_table(&ext_list);
	free(file_name);
	fclose(src);
	fclose(output);
	/*printf("close and free\n");*/

	return err_flag; /* returns the err_flag value */
	
}
