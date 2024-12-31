
#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "line_runner.h"
#include "operation.h"

#include "second_pass.h"

#include <stdio.h>
#include <string.h>


/* ----------------------------------------------- private functions for second pass ----------------------------------------- */

/* this function handle a single operand for the second pass. by the type of the operand the function increment ic and build 
 * a word that contains the address of the label by the function is_build_address_label_word.
 * @param ic: counter instruction
 * @param line_n: the current line number
 * @param instruc_img: the instruction array
 * @param op_type: the address type of the given operand
 * @param operand: the given operand argument
 * @param table: pointer to the symbol table
 * @param ext_list: pointer to list of referred externals symbols
 * return: returns 1 if there is an error else returns 0
*/
int op_handle(int *ic, int line_n, word **instruc_img, adress_type op_type, char *operand, sym_table table, sym_table *ext_list);


/* this function adds the word that contains the label address to the instruction image array
 * @param label_sym: pointer to the label fro the symbol table
 * @param line_n: the current line number
 * @param ic: pointer to instruction counter
 * @param instruc_img: pointer to the istruction image array
 * return: if there is an error oof the label address the function returns 0 else returns 1
*/
int is_build_address_label_word(sym_table label_sym, int line_n, int *ic, word **instruc_img);
/*--------------------------------------------------------------------------------------------------------------------------------*/


int second_pass_line_analyser(char *line, int *ic, word **instruc_img, sym_table *table, sym_table *ext_list){

	char *token; /* token that contains each text word from the current line without white space */
	int err_flag; /* a flag that indicate if there is a syntax error from the source file. sets the flag with false */
	static int line_n = 0; /* the number of the current line */
	char *p_colon; /* pointer to the colon char ':' */
	int label_flag = 0; /* a flag that indicate if their is deffenition of label */

	char label_name[MAX_CHAR_LINE] = {0}; /* string that contain label name */

	char *first_operand; /* the first operand string argument */
	char *second_operand; /* the second operand string argument */
	int operand_counter; /* the amount of operands in the op sentence */

	adress_type type_first; /* the address type of the first operand */
	adress_type type_second; /* the address type of the second operand */

	err_flag = 0;
	++line_n; /* increment the current line number */
	token = strtok(line, " \t\f\r\n"); /* contains the first text word in the current line  that without white space */

	/* condition if the current line is empty sentence */
	if(token == NULL){
		return 0; /* return err_flag with value false and move on to the next line */
	}

	/* condition if the current line is note sentence */
	if(token[0] == ';'){
		return 0; /* return err_flag with value false and move on to the next line */
	}

	/* check if the first text word contain label deffenition by checking if it contain the colon char ':' */
	p_colon = strchr(token, ':');
	if(p_colon != NULL){

		/* the validity of the label deffinition already checked at the first pass so no need to check it again here */

		token = strtok(NULL, " \t\f\r\n"); /* move on to the text word that without white space */

		label_flag = 1; /* turn on the label flag */
	}

	if(strcmp(token,".define") == 0 || strcmp(token,".data") == 0 || strcmp(token,".string") == 0 || strcmp(token,".extern") == 0){
		return 0; /* return err_flag with value false and move on to the next line */
	}

	if(strcmp(token,".entry") == 0){

		/* print warning if there is label deffinition in the current line */
		if(label_flag){
			printf("warning: the label definition with the instruction entry is meaningless in line %d\n",line_n);
		}

		/* i was assumption that there is only one operand of the entry instruction */
		
		token = strtok(NULL, " \t\f\r\n");/* move on to the label operand argument */

		/* token: now the token string contain the operand label argument in the entry sentence */

		strcpy(label_name, token); /* sets the string label_name to the label name of the operand argument  */

		/* assumption that the entry sentence has only one operand argument of label name */
		
		token = strtok(NULL, " \t\f\r\n");/* move on to the text word to check validity */

		/* if there is another argument after the operand argument of the entry sentence */
		if(token != NULL){
			printf("error: too many arguments in the  entry sentence at line %d\n", line_n);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* check if the name of the lable operand argument is legal */
		if(!is_legal_symbol_name(label_name)){
			printf("error: invalid label name for entry sentence in line %d\n", line_n);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* now set the label name with attribute entry */

		/* if the set attribute of the symbol did not succeed */
		if(!is_set_label_entry(table, label_name, line_n)){ /* is_set_label_entry function at symbol_table file */
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}
		
		return err_flag; /* return 0 (false) for no error in the current line */
	}

	


	/* if the text word in the current line is not instruction of data, string, external or entry then is operation sentence */

	operand_counter = 0; /* sets the operand counter to zero */		

	first_operand = (char*)malloc_and_check(MAX_CHAR_LINE);
	second_operand = (char*)malloc_and_check(MAX_CHAR_LINE);

	memset(first_operand,0,MAX_CHAR_LINE);
	memset(second_operand,0,MAX_CHAR_LINE);

	/* now the token move on to the operands */
	token = strtok(NULL, " \t\f\r\n");
	
	/* sets the operands strings and the operand counter */
	set_operands(token, line_n, &first_operand, &second_operand, &operand_counter);

	/* note: */
	/* there is no need to check again the syntax error in the op sentence that the function set_operands does */
	/* but the syntax error checks doesn't effect the time complexity of the function */

		
	/* condition for the group 1 */
	if(operand_counter == 2){

		++(*ic); /* moving  from the first of the operation sentence to the second word */

		type_first = get_address_type(first_operand);
		type_second = get_address_type(second_operand);

		if(type_first == DIRECT_REGISTER_ADDRESS && type_second == DIRECT_REGISTER_ADDRESS){
			/* only one word build */
			++(*ic);
			free(first_operand);
			free(second_operand);
			return 0; /* return the false for no error occure */
		}

		err_flag = op_handle(ic, line_n, instruc_img, type_first, first_operand, *table, ext_list);

		if(err_flag){
			free(first_operand);
			free(second_operand);
			return err_flag;
		}

		err_flag = op_handle(ic, line_n, instruc_img, type_second, second_operand, *table, ext_list);

		free(first_operand);
		free(second_operand);
		return err_flag;
	}



	/* condition for the group 2 */
	if(operand_counter == 1){

		++(*ic); /* moving  from the first of the operation sentence to the second word */

		type_first = get_address_type(first_operand);
		err_flag = op_handle(ic, line_n, instruc_img, type_first, first_operand, *table, ext_list);

		free(first_operand);
		free(second_operand);
		return err_flag;
	}

	/* condition for the group 3 */
	if(operand_counter == 0){
		++(*ic);
		free(first_operand);
		free(second_operand);
		return err_flag; /* return error value */
	}

	return 1;  /* return an error */
}


int op_handle(int *ic, int line_n, word **instruc_img, adress_type op_type, char *operand, sym_table table, sym_table *ext_list){

	char *lable_name; /* string that contains the lable name in case fixed index address  */
	char *cons_name; /* string that contains the constant name of the index in case fixed index address */
	sym_table label_sym; /* the label from the symbol table */

	if(op_type == IMMEDIATE_ADDRESS){
		++(*ic);
		return 0; /* return to err_flag value of false to indicate that there is no error */
	}

	/* in case of addressing type 1: direct address */
	else if(op_type == DIRECT_ADDRESS){

		/* note: the label name valid checked in the get address type */

		label_sym = search_symbol(table, operand); /* get the label from symbol table */

		/* condition if the label name is not in the symbol table */
		if(label_sym == NULL){
			printf("error: undeclared label name in the operation sentence at line %d\n", line_n);
			return 1; /* return the error flag true */
		}

		/* else the label name is found at the symbol table */

		/* condition if the label symbol is external */
		if(label_sym->attribute == external){
			/* add the external symbol to ext list with val of addressing code machine */
			add_symbol(ext_list, operand, *ic+100, external);
		}
		

		/* build the word of the label address and increment ic by 1 through the function is_build_address_label_word */

		/* if there was an error of the word label address to add */
		if(!is_build_address_label_word(label_sym, line_n, ic, instruc_img)){
			return 1; /* return to err_flag value true */
		}
		
		return 0; /* return to err_flag value of false to indicate that there is no error */	
	}

	/* in case of addressing type 1: fixed index address */
	else if(op_type == FIXED_INDEX_ADDRESS){

		/* get the label name string without the index value */
		lable_name = (char*)malloc_and_check(MAX_CHAR_LINE);
		cons_name = (char*)malloc_and_check(MAX_CHAR_LINE);

		memset(lable_name, 0, MAX_CHAR_LINE);
		memset(cons_name, 0, MAX_CHAR_LINE);

		/* the valid of the set already checked at get address type function */
		is_label_and_index_set(&lable_name, &cons_name, operand); 

		/* check the valid of the label name */
		if(!is_legal_symbol_name(lable_name)){
			printf("error: the label name is invalid in line %d\n", line_n);
			free(lable_name);
			free(cons_name);
			return 1; /* return to err_flag value true */
		}

		label_sym = search_symbol(table, lable_name); /* get the label from symbol table */


		/* condition if the label name is not in the symbol table */
		if(label_sym == NULL){
			printf("error: undeclared label name in the operation sentence at line %d\n", line_n);
			free(lable_name);
			free(cons_name);
			return 1; /* return the error flag true */
		}

		/* else the label name is found at the symbol table */

		/* condition if the label symbol is external */
		if(label_sym->attribute == external){
			/* add the external symbol to ext list with val of addressing code machine */
			add_symbol(ext_list, lable_name, *ic+100, external);
		}

		/* build the word of the label address and increment ic by 1 through the function is_build_address_label_word */

		/* if there was an error of the word label address to add */
		if(!is_build_address_label_word(label_sym, line_n, ic, instruc_img)){
			free(lable_name);
			free(cons_name);
			return 1; /* return err_flag value */
		}

		free(lable_name);
		free(cons_name);

		++(*ic); /* increment ic for the word that contains the index value */

		return 0; /* return to err_flag value of false to indicate that there is no error */
	}

	else if(op_type == DIRECT_REGISTER_ADDRESS){
		++(*ic);
		return 0; /* return to err_flag value of false to indicate that there is no error */
	}

	else{
		return 1; /* return the error flag true to indicate that there is an error */
	}

}


int is_build_address_label_word(sym_table label_sym, int line_n, int *ic, word **instruc_img){

	word *w; /* the word of the label address */
	immediate_word *imd_w; /* the type of word of the label address */
	int address_val; /* the address value from the symbol label */

	address_val = label_sym->val;

	w = (word*)malloc_and_check(sizeof(word));
	imd_w = (immediate_word*)malloc_and_check(sizeof(immediate_word));


	if( label_sym->attribute == external ){
		imd_w->ARE = 1;
	}

	else{
		imd_w->ARE = 2;
	}

	imd_w->val = address_val;
	w->op = NULL;
	w->imd = imd_w;
	w->reg = NULL;
	instruc_img[*ic] = w; /* sets the word of that contains the label address in to the insruction image */
	++(*ic); /*increase the instruction counter*/

	return 1;
}



