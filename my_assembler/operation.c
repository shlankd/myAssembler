
#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "operation.h"

#include <stdio.h>
#include <string.h>

/* list of opcodes with their string name */
static struct op_list_node op_list[] = {
	/*contains list of opcodes with opname and their legal addresses types for source (if there is) and destination (if there is) */
	{mov,"mov", {0,1,2,3}, {1,2,3,-1}},
	{cmp,"cmp", {0,1,2,3}, {0,1,2,3}},
	{add,"add", {0,1,2,3}, {1,2,3,-1}},
	{sub,"sub", {0,1,2,3}, {1,2,3,-1}},
	{not,"not", {-1,-1,-1,-1}, {1,2,3,-1}},
	{clr,"clr", {-1,-1,-1,-1}, {1,2,3,-1}},
	{lea,"lea", {1,2,-1,-1}, {1,2,3,-1}},
	{inc,"inc", {-1,-1,-1,-1}, {1,2,3,-1}},
	{dec,"dec", {-1,-1,-1,-1}, {1,2,3,-1}},
	{jmp,"jmp", {-1,-1,-1,-1} , {1,3,-1,-1}},
	{bne,"bne", {-1,-1,-1,-1} , {1,3,-1,-1}},
	{red,"red", {-1,-1,-1,-1}, {1,2,3,-1}},
	{prn,"prn", {-1,-1,-1,-1}, {0,1,2,3}},
	{jsr,"jsr", {-1,-1,-1,-1} , {1,3,-1,-1}},
	{rts,"rts", {-1,-1,-1,-1}, {-1,-1,-1,-1}},
	{hlt,"hlt", {-1,-1,-1,-1}, {-1,-1,-1,-1}},
	{unvalid,"unvalid opname", {-1,-1,-1,-1}, {-1,-1,-1,-1}}/* ????????????? */
};


int set_operands(char *op_sentence, int num_of_line, char **first_operand, char **second_operand, int *op_counter){

	char *p_comma; /* pointer to the comma char ',' */
	int comma_pos; /* the position of the char ',' in the operand argument */



	/* condition for NO operand */
	if(op_sentence == NULL){
		return 0; /* return the error flag false */
	}

	/* check if the op_sentence contain comma ',' */
	p_comma = strchr(op_sentence, ',');

	/* if there is no comma in the first text word after the op name */
	if(p_comma == NULL){

		/* then sets the token op_sentence as first operand */
		strcpy(*first_operand,op_sentence);

		++(*op_counter); /* increase operand counter */

		/* move on to the next token of op_sentence */
		op_sentence = strtok(NULL, " \t\f\r\n");

		/* condition for ONE operand */
		if(op_sentence == NULL){
			return 0; /* return the error flag false */
		}

		/*else there is another text word after first operand */

		/* if the first char of the text word after the first operand doesn't contain comma char */
		if(op_sentence[0] != ','){
			/* then there is an error in the operation sentence */
			printf("error: invalid operation sentence, missing comma between operands in line %d\n", num_of_line);
			return 1; /* return the error flag true */
		}

		/* else there is comma char sets the comma pointer */
		p_comma = strchr(op_sentence, ',');
	}

	/* if there is comma char in the first text word after operation name */
	else if(p_comma != NULL){

		/* if the first char of the text word is comma ',' */
		if(op_sentence[0] == ','){
			printf("error: invalid operation sentence, missing operand argument before comma in line %d\n", num_of_line);
			return 1; /* return the error flag true */
		}

		comma_pos = strcspn(op_sentence, ","); /* set the position of the char comma ',' */

		/* else sets the characters sequence before the comma char in the text word*/
		strncpy(*first_operand, op_sentence, comma_pos);
		++(*op_counter); /* increase operand counter */
	}

	/* if the text word after the first operand contain only comma char
	 * then move on to the next text word to the second operand */
	if(strlen(p_comma) == 1){

		op_sentence = strtok(NULL, " \t\f\r");

		/* if there is no text word after the comma char then there is an error for invalid operand sentence */
		if(op_sentence == NULL){
			printf("error: invalid operation sentence, missing operand argument after comma in line %d\n", num_of_line);
			return 1; /* return the error flag true */
		}
           
		/* else argument exists so sets the text word after the comma char as second operand */
		strcpy(*second_operand, op_sentence);
		++(*op_counter); /* increase operand counter */
	}

	/* else if the the second operand argument consists in the text word */
	else if(strlen(p_comma) > 1){
		++p_comma;

		/* sets the argument after the comma char as the second operand */
		strcpy(*second_operand, p_comma);
		++(*op_counter); /* increase operand counter */
	}

	/* check if there is another text word after the second operand set */
	op_sentence = strtok(NULL, " \t\f\r\n");

	/* condition if a text word exists after the text of the second operand */
	if(op_sentence != NULL){
		printf("error: invalid operation sentence,too many arguments in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	return 0; /* return the error flag false */
}


int is_op_invalid(opcodes opcode, int num_of_line, char *first_operand, char *second_operand, int op_counter, adress_type *types_arr){

	adress_type type_first; /* the address type of the first operand */
	adress_type type_second; /* the address type of the second operand */
	int is_first_legal; /* flag for indicate if the address type of the first operand is legal */
	int is_second_legal; /* flag for indicate if the address type of the second operand is legal */

	/*op_list_node *i;*/ /* pointer to the list of opcodes with string name  */
	
	/* in case the operation is the first group */
	if( (opcode >= mov && opcode <= sub) || opcode == lea ){

		/* condition if the number of operands does not match with the operation group */
		if(op_counter != 2){
			printf("error: this operation sentence is invalid, must have only 2 operands arguments in line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		type_first = get_address_type(first_operand);
		type_second = get_address_type(second_operand);

		if(type_first == ERROR || type_second == ERROR){
			printf("error: the operands are not valid in this operation sentence at line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		/* check if the addresses types of the operands are legal for the current operation */
		is_first_legal = is_legal_address(opcode, type_first, 1); /* check for the addresses types for source operand */
		is_second_legal = is_legal_address(opcode, type_first, 0); /* check for the addresses types for destination operand */

		if(!is_first_legal || !is_second_legal){
			printf("error: the types of the operands are not valid for this operation sentence in line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		types_arr[0] = type_first;
		types_arr[1] = type_second;
	}

	/* in case the operation is the second group */
	else if( (opcode >= not && opcode <= clr) || (opcode >= inc && opcode <= jsr) ){

		/* condition if the number of operands does not match with the operation group */
		if(op_counter != 1){
			printf("error: this operation sentence is invalid, must have only 1 operand argument in line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		type_first = get_address_type(first_operand);

		if(type_first == ERROR){
			printf("error: the operand is not valid in this operation sentence at line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		/* check if the address type of the operand is legal for the current operation */
		is_first_legal = is_legal_address(opcode, type_first, 0); /* check for the addresses types for destination operand */

		if(!is_first_legal){
			printf("error: the type of the operand is not valid for this operation sentence in line %d ",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		types_arr[0] = type_first;
		types_arr[1] = ERROR; 
	}

	/* in case the operation is the third group */
	else if( opcode >= rts && opcode <= hlt ){

		/* condition if the number of operands does not match with the operation group */
		if(op_counter != 0){
			printf("error: this operation sentence is invalid, must have 0 operands arguments in line %d\n",num_of_line);
			return 1; /* return 1 to set the err_flag true */
		}

		types_arr[0] = ERROR;
		types_arr[1] = ERROR;
	}
	return 0; /* return false for no error counter */
}


adress_type get_address_type(char *operand_arg){

	char *lable_name; /* string that contains the lable name in case fixed index address  */
	char *cons_name; /* string that contains the constant name of the index in case fixed index address */

	if(operand_arg == NULL){
		return ERROR;
	}


	lable_name = (char*)malloc_and_check(MAX_CHAR_LINE);
	cons_name = (char*)malloc_and_check(MAX_CHAR_LINE);

	memset(lable_name, 0, MAX_CHAR_LINE);
	memset(cons_name, 0, MAX_CHAR_LINE);

	/* in case of addressing type 0: immediate address */
	if(operand_arg[0] == '#'){
		++operand_arg;
		if( is_legal_symbol_name(operand_arg) || is_num_valid(operand_arg) ){
			free(lable_name);
			free(cons_name);
			return IMMEDIATE_ADDRESS;
		}
	}

	/* in case of addressing type 1: direct address */
	else if(is_legal_symbol_name(operand_arg)){
			free(lable_name);
			free(cons_name);
		return DIRECT_ADDRESS;
	}

	/* in case of addressing type 2: fixed index address */
	else if( is_label_and_index_set(&lable_name, &cons_name, operand_arg) ){ /* if succeed to set */

		/* check the argument validity of the label name and the argument validity of the index */
		/*if(!is_legal_symbol_name(lable_name) || (!is_legal_symbol_name(cons_name) && !is_num_valid(cons_name))){
			free(lable_name);
			free(cons_name);
			return ERROR;
		}*/

		free(lable_name);
		free(cons_name);
		return FIXED_INDEX_ADDRESS;
	}

	/* in case of addressing type 3: direct register addressing */
	else if(is_register_name(operand_arg)){
			free(lable_name);
			free(cons_name);
		return DIRECT_REGISTER_ADDRESS;
	}

	free(lable_name);
	free(cons_name);
	return ERROR; /*????????????????*/

}


void set_opcode(char *op_str, opcodes *set_op){

	op_list_node *i; /* pointer to the list of opcodes with string name  */

	*set_op = unvalid; /* set unvalid if the op_str does not contain an opname */

	for(i=op_list; i->opcode!=unvalid; i++){
		if(strcmp(op_str,i->opname) == 0){
			*set_op = i->opcode;
		}
	}
}


int is_legal_address(opcodes curr_op, adress_type ad_type, int is_source_operand){

	/* pointer to the list of opcodes with string name and the legal addresses types for the source and destination operands */
	op_list_node *op; 
	int i; /* iterator */

	for(op=op_list; op->opcode!=unvalid; op++){

		if(op->opcode == curr_op){
			i = 0;
			if(is_source_operand){

				/* loop that runs through the addresses types that support for the current operation */
				while( (i<4) && (op->src_operand[i] != -1) ){
					if(op->src_operand[i] == ad_type){
						return 1; /* returns true for a valid address type for this current operation*/
					}
					++i;
				}
			}

			else{

				/* loop that runs through the addresses types that support for the current operation */
				while( (i<4) && (op->des_operand[i] != -1) ){
					if(op->des_operand[i] == ad_type){
						return 1; /* returns true for a valid address type for this current operation*/
					}
					++i;
				}
			}		
		}
	}

	return 0;

}








