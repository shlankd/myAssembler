
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "functions_with_check.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *malloc_and_check(long size){
	void *ptr; /* generic pointer */

	ptr = malloc(size); /* point to the allocate memory */

	/* condition for failure to allocate memory */
	if(ptr == NULL){
		printf("Error: could not allocate memory\n");
		exit(1);
	}
	return ptr;
}

FILE *fopen_and_check(const char *file_name, const char *mode_operation){

	FILE *fp;

	fp = fopen(file_name, mode_operation);
	
	if(fp == NULL){
		printf("Error: could not open the file %s\n", file_name);
	}

	return fp;
}

int is_imd_overflow(int check_num, int num_line){

	if(check_num > MAX_INT_IMD || check_num < MIN_INT_IMD){
		printf("error: the input integer in line %d will cause overflow\n",num_line);
		return 1;
	}

	return 0;
}

int is_data_overflow(int check_num, int num_line){

	if(check_num > MAX_INT_DATA || check_num < MIN_INT_DATA){
		printf("error: the input integer in line %d will cause overflow\n",num_line);
		return 1;
	}

	return 0;
}


int is_label_and_index_set(char **label_name, char **ind, char *operand_arg){

	char *obp; /* pointer to the open brackets char '[' */
	char *cbp; /* pointer to the close brackets char ']' */
	int obp_pos; /* the position of the '[' char */
	int cbp_pos; /* the position of the ']' char */

	if(operand_arg == NULL ){
		return 0; /* return failure to set  */
	}

	obp = strchr(operand_arg,'['); /* sets the pointer char to '[' */

	/* if the char '[' don't exsits or there is no argument after '[' then returns false */
	if(obp == NULL ){
		return 0; /* return failure to set  */
	}

	obp_pos = strcspn(operand_arg, "["); /* sets the position of '[' */

	/* if the char '[' apears as the first char of the operand argument then the label name is missing' */
	if(obp_pos == 0 ){
		return 0; /* return failure to set  */
	}

	++obp; /* sets the pointer to be after the char '[' */

	cbp = strchr(obp, ']'); /* sets the string of constant value */

	/* if the char ']' don't exsits or there is argument after ']' */
	if(cbp == NULL || strlen(cbp)>1){
		return 0; /* return failure to set  */
	}

	cbp_pos = strcspn(obp, "]"); /* sets the position of ']' */

	strncpy(*label_name, operand_arg, obp_pos); /* sets the string label name */
	strncpy(*ind, obp, cbp_pos); /* sets the string constant name */

		
	return 1; /* return succsess to set the 2 strings */
}

/* this function free the instruction image */
void free_instruction_image(word **instruct_img, int size){
	int i;
	for(i=0; i<size; i++){
		if(instruct_img[i] != NULL){

			if(instruct_img[i]->op != NULL){
				free(instruct_img[i]->op);
			}

			else if(instruct_img[i]->imd != NULL){
				free(instruct_img[i]->imd);
			}

			else if(instruct_img[i]->reg != NULL){
				free(instruct_img[i]->reg);
			}
			free(instruct_img[i]);
		}
	}
}

/* this function free the data image */
void free_data_image(data_word **data_img, int size){
	int i;
	for(i=0; i<size; i++){
		if(data_img[i] != NULL){
			free(data_img[i]);
		}
	}
}

/* ----------------------------------TO DELETE---------------------------------- */
void print_file(FILE *fp){
	char c;
	while(!feof(fp)){
		putchar((c = fgetc(fp))); /* putchar(c) equivalent to putc(c, stdout) */
	}
	rewind(fp);
}
/* ----------------------------------TO DELETE---------------------------------- */




