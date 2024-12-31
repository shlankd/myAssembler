
#include "first_pass.h"
#include "constants.h"
#include "macro_data.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "functions_with_check.h"
#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this private functions are used for create words that contains operation instructions. 
 * these functions called after the validity check of the operation sentence  */

/* this function builds extra word given the operand argument
 * @param ic: counter instruction
 * @param line_num: the current line number
 * @param instruc_img: the instruction array
 * @param op_type: the address type of the given operand
 * @param operand: the given operand argument
 * @param is_reg_source: a flag to indicate in case of addressing type 3 if the given operand argument is source register or not
 * @param table: pointer to the symbol table
 * return: returns 1 if there is an error else returns 0
 */
int build_extra_word(int *ic, int line_num, word **instruc_img, adress_type op_type, char *operand, int is_reg_source, sym_table *table);

/* this function adds one extra to the instruction image when operation of first group has two operands of immediate register type */
void one_extra_reg_word(int *ic, word **instruc_img, char *first_operand, char *second_operand);

/* functions that returns the first word of each operation group */
operation_word *get_group1_first_word(opcodes opcode, adress_type *types);
operation_word *get_group2_first_word(opcodes opcode, adress_type *types);
operation_word *get_group3_first_word(opcodes opcode);

int first_pass_line_analyser(char *line, char *copy_line, int *ic, int *dc, word **instruc_img, data_word **data_img, sym_table *table, ptr_mcr m_list){

	char *token; /* token that contains each text word from the current line without white space */
	int err_flag; /* a flag that indicate if there is a syntax error from the source file. sets the flag with false */
	static int num_of_line = 0; /* the number of the current line */
	int label_flag = 0; /* a flag that indicate if their is deffenition of label */

	char *first_operand;/* string that contains the first operand argument */
	char *second_operand; /* string that contains the second operand argument */
	char op_name[5] = {0}; /* string that contains the operation name */
	int operand_counter; /* operand counter */
	adress_type types[2]; /* the operands addressing type when types[0] is for the first operand and types[1] is for the second */
	opcodes opcode; /* the opcode of the current line if exists */
	word *first_word; /* the first word of a certain insruction */

	char *p_colon; /* pointer to the colon char ':' */
	char lable_deff[MAX_CHAR_LINE] = {0};/* a string that contains the name of the lable that gets definition in the current line */
	int colon_pos; /* the position of the char colon ':' in the label definition */

	err_flag = 0;
	++num_of_line; /* increment the current line number */
	token = strtok(copy_line, " \t\f\r\n"); /* contains the first text word in the current line  that without white space */

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
		/* if the first char of the first text word is colon ':' then it's empty definition of label. */
		if(token[0] == ':'){
			printf("error: invalid label defenition, missing label argument before ':' in line %d\n", num_of_line);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* if there is another chars that not white space after the char ':' */
		if(strlen(p_colon) > 1){
			printf("error: invalid label defenition, there is arguments after ':' in line %d\n", num_of_line);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* else sets the characters sequence before the colon char of the first text word*/
		colon_pos = strcspn(token, ":"); /* the position of the char ':' in the first text word */
		strncpy(lable_deff, token, colon_pos); /* sets the string lable_deff to the lable name */

		/* check if the name of the lable definition is legal */
		if(!is_legal_symbol_name(lable_deff)){
			printf("error: invalid label name in line %d\n", num_of_line);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* check if the name of the lable definition is exsits in the symbol table */
		if(is_symbol_exsits(*table, lable_deff)){
			printf("error: the definition label name is already exists in line %d \n", num_of_line);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* check if the name of the lable definition is exsits in the list of macro */
		if(is_macro_name_exists(m_list, lable_deff)){
			printf("error: the definition label name is already exists as macro in line %d \n", num_of_line);
			err_flag = 1;
			return err_flag; /* return the error flag true */
		}

		/* if there is no error of the lable definition then turn on the lable flag to indicate od label definition */
		label_flag = 1;

		/*MOVE ON TO THE NEXT TEXT WORD*/
		token = strtok(NULL, " \t\f\r\n"); /* move on to the next text word from the current line */
	}

	/* condition if the text word in the current line is .define */
	if( strcmp(token,".define") == 0 ){
		token = strtok(NULL, " \t\f\r\n"); /* move to the next text word from the current line */
		err_flag = define_sentence(token, num_of_line, table);
		return err_flag; /* return error value */
	}

	/* condition if the text word in the current line is .data */
	else if( strcmp(token,".data") == 0 ){

		/* if the label flag is on then add the label name definition to the symbol table with attribute of data  */
		if(label_flag){
			add_symbol(table, lable_deff, *dc, data);
		}

		token = strtok(NULL, " \t\f\r\n"); /* move to the next text word from the current line */
		err_flag = data_sentence(token, num_of_line, dc, data_img, table);
		return err_flag; /* return error value */
	}

	/* condition if the text word in the current line is .string */
	else if( strcmp(token,".string") == 0 ){

		/* if the label flag is on then add the label name definition to the symbol table with attribute of data */
		if(label_flag){
			add_symbol(table, lable_deff, *dc, data);
		}

		token = strtok(NULL, " \t\f\r\n"); /* move to the next text word from the current line */
		err_flag = string_sentence(line, token, num_of_line, dc, data_img); 
		return err_flag; /* return error value */
	}

	/* condition if the text word in the current line is .extern */
	else if( strcmp(token, ".extern") == 0 ){

		/* if the label flag on prints warning messege for no use of the label definition */
		if(label_flag){
			printf("warning: the label definition with the instruction extern is meaningless in line %d\n",num_of_line);
		}

		token = strtok(NULL, " \t\f\r\n"); /* move to the next text word from the current line */
		err_flag = external_sentence(token, num_of_line, table);
		return err_flag; /* return error value */
	}

	/* condition if the text word in the current line is .entry */
	else if( strcmp(token, ".entry") == 0 ){
		return err_flag;
	}

	/* condition if the text word in the current line is instruction operation sentence */
	else if( is_op_name(token) ){

		/* if the label flag is on then add the label name definition to the symbol table with attribute of code */
		if(label_flag){
			add_symbol(table, lable_deff, *ic+100, code);
		}

		operand_counter = 0; /* sets the operand counter to zero */

		strcpy(op_name, token); /* sets the current operation name string */
		set_opcode(op_name, &opcode); /* sets the opcode of the current operation */
		

		first_operand = (char*)malloc_and_check(MAX_CHAR_LINE);
		second_operand = (char*)malloc_and_check(MAX_CHAR_LINE);

		memset(first_operand,0,MAX_CHAR_LINE);
		memset(second_operand,0,MAX_CHAR_LINE);

		/* now the token move on to the operands */
		token = strtok(NULL, " \t\f\r\n");
	
		/* sets the operands strings and the operand counter */
		err_flag = set_operands(token, num_of_line, &first_operand, &second_operand, &operand_counter);

		if(err_flag){
			free(first_operand);
			free(second_operand);
			return err_flag; /* return error value */
		}

		/* check the valid of operands and sets their addressing types */
		err_flag = is_op_invalid(opcode, num_of_line, first_operand, second_operand, operand_counter, types);

		if(err_flag){
			free(first_operand);
			free(second_operand);
			return err_flag; /* return error value */
		}



		first_word = (word*)malloc_and_check(sizeof(word));
		
		/* condition for the group 1 */
		if(operand_counter == 2){

			first_word->op = get_group1_first_word(opcode,types);
			first_word->imd = NULL;
			first_word->reg = NULL;
			instruc_img[*ic] = first_word;
			++(*ic);

			if(types[0] == DIRECT_REGISTER_ADDRESS && types[1] == DIRECT_REGISTER_ADDRESS){

				/*only 1 extra word for direct register address of two operands source and destination)*/
				one_extra_reg_word(ic, instruc_img, first_operand, second_operand);
			}

			else{
				/* sets the extra words to each operand */
				err_flag = build_extra_word(ic, num_of_line, instruc_img, types[0], first_operand, 1, table);
				if(err_flag){
					free(first_operand);
					free(second_operand);
					return err_flag; /* return error value */
				}

				err_flag= build_extra_word(ic, num_of_line, instruc_img, types[1], second_operand, 0, table);
				if(err_flag){
					free(first_operand);
					free(second_operand);
					return err_flag; /* return error value */
				}
			}	
		}

		/* condition for the group 2 */
		if(operand_counter == 1){


			first_word->op = get_group2_first_word(opcode,types);
			first_word->imd = NULL;
			first_word->reg = NULL;
			instruc_img[*ic] = first_word;
			++(*ic);

			/* sets the extra word of the operand */
			err_flag = build_extra_word(ic, num_of_line, instruc_img, types[0], first_operand, 0, table);
			if(err_flag){
				free(first_operand);
				free(second_operand);
				return err_flag; /* return error value */
			}
		}

		/* condition for the group 3 */
		if(operand_counter == 0){

			first_word->op = get_group3_first_word(opcode);
			first_word->imd = NULL;
			first_word->reg = NULL;
			instruc_img[*ic] = first_word;
			++(*ic);
		}

		free(first_operand);
		free(second_operand);
		return err_flag; /* return error value */
	}

	/* if the current line does not applied to any conditions then it contains an undefined sentence */
	printf("invalid sentence due to undefined argumentes in line %d\n",num_of_line);
	err_flag = 1; /* sets the error flag true */
	return err_flag; /* return error value */
	
}


int build_extra_word(int *ic, int line_num, word **instruc_img, adress_type op_type, char *operand, int is_reg_source, sym_table *table){

	if(op_type == IMMEDIATE_ADDRESS){
		word *imd_w; /* the word to set */
		immediate_word *imd_num;/* the data word to set */
		int val; /* the val to set in the data word */
		unsigned int mask;

		/* sets the operand argumnt to the value argument after the char '#' */
		++operand;

		/* if the immediate operand contains an integer */
		if(is_num_valid(operand)){
			val = atoi(operand); /* need to check overflow */
			if( is_imd_overflow(val,line_num) ){ /* if the immediate operand contains number that can cause overflow */
				return 1; /* the return 1 to indicate an error */
			}
		}

		/* else the immediate operand contains a define lable */
		else{
			sym_table defined_lable;
			defined_lable = search_symbol(*table, operand);

			if(defined_lable == NULL || defined_lable->attribute != mdefine){
				printf("error: undefined constant lable in line %d\n",line_num);
				return 1; /* return 1 to indicate an error */
			}
			val = defined_lable->val; /* already checked overflow before enter to the symbol table  */
		}

		/* the data val is valid, so sets the word immediate with the data of val and the word that contain the data word */
		imd_w = (word*)malloc_and_check(sizeof(word));
		imd_num = (immediate_word*)malloc_and_check(sizeof(immediate_word));
		mask = 4095; /* mask = 4095 = 0xfff = 1111 1111 1111 */
		imd_num->ARE = 0;
		imd_num->val = val&mask; /* takes only the first 12 bits (from the right) of the value of immediate */
		imd_w->op = NULL;
		imd_w->imd = imd_num;
		imd_w->reg = NULL;
		instruc_img[*ic] = imd_w; /* sets the data word of the immediate operand in to the insruction image */
		++(*ic); /*increase the instruction counter*/

		return 0; /* return 0 to indicate that the extara word of the operand sets withot an error */
	}

	if(op_type == DIRECT_ADDRESS){
		/* save the current ic slot for the word that contains the address of the label */
		++(*ic); /* increase the instruction counter */

		return 0; /* return 0 to indicate that the extara word of the operand sets withot an error */
	}

	if(op_type == FIXED_INDEX_ADDRESS){

		char *label; /* contains the sting of the label name */
		char *index; /* contains the string of the index */
		int index_val; /* the index value */
		sym_table defined_sym; /* a symbol pointer that with attribute mdefine */
		word *ind_w; /* the word to set */
		immediate_word *ind_num;/* the data word to set */
		unsigned int mask_index;
		
		/* the second pass will code the address label */
		++(*ic); /* save the slot for the address code of the label */

		/* allocate memory to the strings to set them with the arguments */
		label = (char*)malloc_and_check(MAX_CHAR_LINE);
		index = (char*)malloc_and_check(MAX_CHAR_LINE);

		memset(label, 0, MAX_CHAR_LINE);
		memset(index, 0, MAX_CHAR_LINE);

		/* sets the label name string and the index string arguments */
		is_label_and_index_set(&label, &index, operand);

		/* the result of the sets strings is known from is_op_invalid function that called in line 251 */
		/* so no need to check if the sets succeeded cause address type of the operand would be an ERROR */

		/* cases of the index argument could be: */

		/* in case the index argument is an integer */
		if(is_num_valid(index)){

			index_val = atoi(index); /* set the index val if an integer */

			/* condition if the input integer for the index value is negative */
			if(index_val < 0){
				free(label);
				free(index);
				printf("error: a negative value is given for an index in line %d\n",line_num);
				return 1; /* return 1 to indicate an error */
			}

			/* condition if the input integer for the index value will cause over flow */
			if(is_imd_overflow(index_val, line_num)){
				free(label);
				free(index);
				return 1; /* return 1 to indicate an error */
			}
		}

		/* in case the index argument is not an integer */
		else{
			if(!is_legal_symbol_name(index)){
				free(label);
				free(index);
				printf("error: invalid operand argument in line %d\n",line_num);
				return 1; /* return 1 to indicate an error */
			}

			/* search for the symbol that with the name of the index argument */
			defined_sym = search_symbol(*table, index);

			if(defined_sym == NULL || defined_sym->attribute != mdefine){
				free(label);
				free(index);
				printf("error: undefined constant lable for an index in line %d\n",line_num);
				return 1; /* return 1 to indicate an error */
			}

			index_val = defined_sym->val; /* already checked overflow before enter to the symbol table  */

			/* condition if the defined constant for the index value will cause over flow */
			if(is_imd_overflow(index_val, line_num)){
				free(label);
				free(index);
				return 1; /* return 1 to indicate an error */
			}
		}
		free(label);
		free(index);

		ind_w = (word*)malloc_and_check(sizeof(word));
		ind_num = (immediate_word*)malloc_and_check(sizeof(immediate_word));
		mask_index = 4095; /* mask = 4095 = 0xfff = 1111 1111 1111 */
		ind_num->ARE = 0;
		ind_num->val = index_val&mask_index; /* takes only the first 12 bits (from the right) of the value of immediate */
		ind_w->op = NULL;
		ind_w->imd = ind_num;
		ind_w->reg = NULL;
		instruc_img[*ic] = ind_w; /* sets the data word of the immediate operand in to the insruction image */
		++(*ic); /* increase the instruction counter */


		return 0; /* return 0 to indicate that the extara word of the operand sets withot an error */
	}

	if(op_type == DIRECT_REGISTER_ADDRESS){
		/* the valid of the operand of this address type already checked at is_op_invalid function from the operation file */
		word *entery_reg_w;
		reg_word *reg_w;
		int reg_val;
		entery_reg_w = (word*)malloc_and_check(sizeof(word));
		reg_w = (reg_word*)malloc_and_check(sizeof(reg_word));
		reg_val = atoi(++operand); /* the value of the operand register that in range 0 to 7 */

		if(is_reg_source){
			reg_w->ARE = 0;
			reg_w->reg_destenation = 0;
			reg_w->reg_source = reg_val;
			reg_w->unuse = 0;
		}
		
		else{
			reg_w->ARE = 0;
			reg_w->reg_destenation = reg_val;
			reg_w->reg_source = 0;
			reg_w->unuse = 0;
		}

		entery_reg_w->op = NULL;
		entery_reg_w->imd = NULL;
		entery_reg_w->reg = reg_w;
		instruc_img[*ic] = entery_reg_w; /* sets the reg word operand in to the insruction image */
		++(*ic); /*increase the instruction counter*/

		return 0; /* return 0 to indicate that the extara word of the operand sets withot an error */
	}
	return 1;  /* return 1 to indicate an error if the input operand address type doesn't match to any address types  */
}

void one_extra_reg_word(int *ic, word **instruc_img, char *first_operand, char *second_operand){

	/* this function only use for operation of first group with two operands address type 3
	 * so the adresses of the operands are known ad type 3 and their validity of the operands */
	
	reg_word *r_w; 
	word *entery_w; 

	entery_w = (word*)malloc_and_check(sizeof(word));
	r_w = (reg_word*)malloc_and_check(sizeof(reg_word));

	r_w->ARE = 0;
	r_w->reg_destenation = atoi(++second_operand);
	r_w->reg_source = atoi(++first_operand);
	r_w->unuse = 0;

	entery_w->op = NULL;
	entery_w->imd = NULL;
	entery_w->reg = r_w;

	instruc_img[*ic] = entery_w; /* sets the reg word operand in to the insruction image */
	++(*ic); /*increase the instruction counter*/
}


operation_word *get_group1_first_word(opcodes opcode, adress_type *types){
	operation_word *op_word;
	op_word = (operation_word*)malloc_and_check(sizeof(operation_word));
	op_word->not_used = 0;
	op_word->opcode = opcode;
	op_word->source_operand = types[0];
	op_word->destenation_operand = types[1];
	op_word->ARE = 0;
	return op_word;
}

operation_word *get_group2_first_word(opcodes opcode, adress_type *types){
	operation_word *op_word;
	op_word = (operation_word*)malloc_and_check(sizeof(operation_word));
	op_word->not_used = 0;
	op_word->opcode = opcode;
	op_word->source_operand = 0;
	op_word->destenation_operand = types[0];
	op_word->ARE = 0;
	return op_word;
}

operation_word *get_group3_first_word(opcodes opcode){
	operation_word *op_word;
	op_word = (operation_word*)malloc_and_check(sizeof(operation_word));
	op_word->not_used = 0;
	op_word->opcode = opcode;
	op_word->source_operand = 0;
	op_word->destenation_operand = 0;
	op_word->ARE = 0;
	return op_word;
}







