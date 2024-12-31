

#include "constants.h"
#include "macro_data.h"
#include "pre_assembler.h"


#include <stdio.h>
#include <string.h>


/* this file implements the declerations functions from the pre_assumbler.h */

void pre_assem_line_runner(FILE *src, FILE *des, ptr_mcr *list){

	char src_line[MAX_CHAR_LINE]; /* line text from the source file */
	char copy_line[MAX_CHAR_LINE]; /* the copy line from the source file */
	char c; /* character from the source file */
	int i; /* iterator for char array */

	memset(src_line, 0, MAX_CHAR_LINE);
	memset(copy_line, 0, MAX_CHAR_LINE);
	i = 0;

	while( (c = fgetc(src)) != EOF ){

	/* assumption that number of characters in line from the source file are correct with maximum 81 characters (include '\n') */
		
		/* Condition for not go out of boundery of the char array src */
		if(i == MAX_CHAR_LINE-1){
			c = '\n';
		}

		src_line[i] = c;

		/* condition to the end of line from source file */
		if(c == '\n'){

			strncpy(copy_line, src_line, i);			
			pre_assem_line_analyser(src_line, copy_line, src, des, list);

			memset(src_line, 0, MAX_CHAR_LINE);
			memset(copy_line, 0, MAX_CHAR_LINE);
			i = 0;
		}
		else{
			++i;
		}
	}

	/* condition when the loop ended by EOF and read some characters from the source file */
	if(i > 0){
		strcpy(copy_line, src_line);
		pre_assem_line_analyser(src_line, copy_line, src, des, list);
	}
}

void pre_assem_line_analyser(char *line, char *copy_line, FILE *src, FILE *des, ptr_mcr *list){

	static char macro_name[MAX_CHAR_LEN_LABLE] = {0}; /* string of the macro name */
	static long macro_start = 0; /* the start possition of the macro defenition in the source file */
	static long macro_end = 0; /* the end possition of the macro defenition in the source file */
	static int flag_mcr = 0; /* a flag that indicate if there is reading of macro defenition (mcr) */
	char *token; /* token that contains each text word from the copy_line */
	ptr_mcr m; /* a macro pointer from the list of macro*/

	token = strtok(copy_line, " \t\f\r\n"); /* gets the first text word from the current line */
	
	if(token == NULL){
		return;
	}

	/* condition when the current line is in the macro defenition */
	if(flag_mcr){

		/* condition for the enf of maceo defenition */
		if(strcmp(token, "endmcr") == 0){

			macro_end = ftell(src) - strlen(line); /* get the position of the end macro defenition */
			add_macro(list, macro_name,macro_start, macro_end); /* add the new macro to the list of macro */
			macro_start = 0; /* set the start position to default value */
			macro_end = 0; /* set the end position to default value */
			memset(macro_name, 0, MAX_CHAR_LEN_LABLE); /* clear the string of the macro name */
			flag_mcr = 0; /* turn off the flag of mcr */
		}
	}

	/* when the current line is NOT in the macro defenition */
	else{

		/* condition when there is a start of macro defenition */
		if(strcmp(token,"mcr") == 0){
			
			token = strtok(NULL, " \t\f\r\n"); /* move the token for getting the macro name */

			/* condition if token NUll */
			if(token == NULL){
				/* print an error for no macro name when there is a start of macro defenition */
				printf("ERROR IN PRE ASSUMBLER: NO NAME FOR THE DEFFENITION MACRO\n");
				return;
			}
		
			strcpy(macro_name,token); /* copy the name of the macro to the macro_name string */
			macro_start = ftell(src); /* get the position of the start macro defenition */
			flag_mcr = 1; /* turn on the flag of mcr */
		}

		/* when there is NOT a start of macro defenition */
		else{

			/* search in the macro list if the token contain a macro name that need to expand */
			m = search_macro_node(*list, token);

			/* condition when the token contain a macro name */
			if(m != NULL){
				long last_curr_position; /* the current position of the source file */
				long macro_length; /* the length of macro defenition */
				int c; /* characters from the macro defenition to print to the destenation file*/
				last_curr_position = ftell(src); /*saving the current position of the source file */
				macro_length = m->end_mcr - m->start_mcr; /* calculate the length of macro defenition */
				
				/* set the pointer position of the source file to the position of the start macro defenition */
				fseek(src, m->start_mcr, SEEK_SET);

				while( (macro_length > 0) &&((c = fgetc(src)) != EOF) ){
					fputc(c,des); /* write the character from the macro defenition to the destenation file */
					--macro_length;
				}

				/* set the pointer position of the source file back to the last current position */
				fseek(src, last_curr_position, SEEK_SET);
			}

			/* condition for write the original line when the line is not in macro defenition 
			   or not consits a macro name or not a start of macro defenition ("mcr") */
			else{
				fprintf(des, "%s", line);
			}
		}
	}

}



