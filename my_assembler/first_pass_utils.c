
#include "first_pass_utils.h"
#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "functions_with_check.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int external_sentence(char *ext_label, int num_of_line, sym_table *table){

	/* assumption that in the .extern instruction there is only one label as operand */

	char lable_name[MAX_CHAR_LINE] = {0}; /*the label name in the instruction .extern */

	/* if there is no text word after .extern in the source file */
	if( ext_label == NULL ){
		printf("error: empty sentence of instruction .extern in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	strcpy(lable_name, ext_label); /* sets the label name string */

	ext_label = strtok(NULL, " \t\f\r\n"); /* move to the next text word from the current line */

	/* with the assumption of one label as operand check if there more arguments after the label operand */
	/*??????*/if(ext_label != NULL){
		printf("error: too many arguments in the sentence of .extern in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	/* if the label name of the external not legal */
	if( !(is_legal_symbol_name(lable_name)) ){
		printf("error: invalid external label name in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	/* check if the name of the lable in the extern instruction is exsits in the symbol table */
	if(is_symbol_exsits(*table, lable_name)){
		printf("error: the label name in the extern instruction in line %d is already exists\n", num_of_line);
		return 1; /* return the error flag true */
	}

	/* add the external lable to the symbol table with attribute external and with value 0 */
	add_symbol(table, lable_name, 0, external);

	return 0; /* return the error flag faulse */
}


int string_sentence(char *line, char *str_deff, int num_of_line, int *dc, data_word **data_img){

	char *fqp; /* pointer to the first quotation mark char '"' in the current line */
	char *lqp; /* pointer to the last quotation mark char '"' in the current line */
	int i; /* string iterator */
	int j; /* string iterator */
	char *tok; /* a pointer char to check if there is text word after the closing quotation mark */
	char string_input[MAX_CHAR_LINE] = {0}; /* contains the string argument */
	char rest_string[MAX_CHAR_LINE] = {0}; /* string that contains the rest of argument of the current line after the last quotation */
	int ascii_val;
	data_word *data_w;

	/* if there is no text word after .string in the source file */
	if( str_deff == NULL ){
		printf("error: empty sentence of instruction string in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	/* if the first char of the first text word after .string is not the char '"' */
	if(str_deff[0] != '"'){
		printf("error: invalid string sentence, missing openning quotation mark in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	fqp = strchr(line,'"'); /* sets fqp from the first apear of quotation mark in the current line */
	lqp = strrchr(line,'"'); /* sets fqp from the last apear of quotation mark in the current line */

	/* condition if the first apear of quotation mark is the same as the last apear of quotation mark */
	if(fqp == lqp){
		printf("error: invalid string sentence, only one quotation mark in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	++fqp;

	/* loop to set the strin argument */
	for(i=0; fqp+i<lqp;i++){
		string_input[i] = *(fqp+i);
		/* condition if the input char is not printable */
		if(!isprint(string_input[i])){
			printf("error: the input char is not printable in line %d\n",num_of_line);
			return 1; /* return the error flag true */
		}
	}

	++lqp; /* sets lqp after the closing quotation mark */
	strcpy(rest_string,lqp);
	tok = strtok(rest_string, " \t\f\r\n"); /* get the text word after the quotation mark */

	/* if there is another text word after the quotation mark */
	if(tok != NULL){
		printf("error: invalid string sentence, arguments apear after the quotation mark in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	/* runs every char of the input string */
	for(j=0; j<strlen(string_input); j++){
		ascii_val = string_input[j]; /* sets the ascii code char val from the string */
		data_w = (data_word*)malloc_and_check(sizeof(data_word));
		data_w->data_val = ascii_val; /* no need to check overflow for ascii codes */
		data_img[*dc] = data_w; 
		++(*dc); /* increment the data counter */
	}

	/* after getting all the chars from the input string */
	ascii_val = '\0'; /* sets the ascii code char val to the ascii code'\0' */
	data_w = (data_word*)malloc_and_check(sizeof(data_word));
	data_w->data_val = ascii_val;  /* no need to check overflow for ascii codes */
	data_img[*dc] = data_w; 
	++(*dc); /* increment the data counter */

	

	return 0; /* return the error flag faulse */
}


int data_sentence(char *data_vals, int num_of_line, int *dc, data_word **data_img, sym_table *table){

	int expect_comma; /* a flag that indicate if the char comma (',') is expected */
	int space_occour; /* flag that indicate if space occour while read an integer data */
	int i; /* string iterator for data_vals */
	int j; /* string iterator for a single data parameter */
	char *last_data_vals; /* the last text word in the data sentence */
	char val_str[MAX_CHAR_LINE] = {0}; /* string that contains an integer value */
	
	/* if there is no text word after .data in the source file */
	if( data_vals == NULL ){
		printf("error: empty sentence of instruction data in line %d\n", num_of_line);
		return 1; /* return the error flag true */
	}

	expect_comma = 0; /* sets the comma flag char to not expected */
	j = 0;
	space_occour = 0; /* sets the space flag faulse and ignore the white space before the first element of the data instruction */

	/* loop for each text word with no white space in the current line */
	while(data_vals != NULL){

		/* loop that runs through the text word */
		for(i=0; i<strlen(data_vals); i++){

			/* if in the text word there is unexpected comma char print an error and continue to the next line */
			if( (data_vals[i] == ',') &&  (!expect_comma) ){
				printf("error: unexpected comma in data sentence in line %d\n",num_of_line);
				return 1; /* return the error flag true */
			}

			/* if in the text word there is expected comma char then check the val string valid */
			else if( (data_vals[i] == ',') && (expect_comma)){
				
				/* try to apply the data parameter and if is not valid */
				if( !is_data_applied(val_str, num_of_line, dc, data_img, table) ){
					return 1; /* then return the error flag true */
				}

				/* else reset val string for the next data parameter */
				j = 0;
				memset(val_str, 0, MAX_CHAR_LINE); /* initiate the value string for the next data input */
				expect_comma = 0; /* sets the comma flag char to not expected */

				/* if the flag space is on */
				if(space_occour){
					space_occour = 0; /* turn off the space flag  */
				}
			}

			/* an error if the current char is not acomma and occur space while read an integer */
			else if(  (data_vals[i] != ',') && (space_occour) && (val_str[0] != 0) ){
				printf("error: a space occure instead comma in the data sentence in line %d\n",num_of_line);
				return 1; /* return the error flag true */
			}
	
			/* if there is no comma char in the text word then read the integer value and set it into the val string */
			else if( (data_vals[i] != ',') ){
				val_str[j] = data_vals[i];
				++j;
				
				if(!expect_comma){
					expect_comma = 1; /* sets the comma flag char to be expected if it is off */
				}	

				/* if the flag space is on */
				if(space_occour){
					space_occour = 0; /* turn off the space flag  */
				}			
			}

		}/* end for loop */

		last_data_vals = data_vals;
		data_vals = strtok(NULL, " \t\f\r\n"); /* move to the next textword with no white space */
		space_occour = 1; /* sets the flag space on */
	} /* end while loop */

	/* check if the data sentence ends with the comma char */
	if(last_data_vals[strlen(last_data_vals)-1] == ','){
		printf("error: unexpected comma in data sensence in line %d\n",num_of_line);
		return 1; /* return the error flag true */
	}

	/* else if the data sentence not ended with comma then adds the last data parameter */

	/* try to aplly the data parameter. if is not valid */
	if( !is_data_applied(val_str, num_of_line, dc, data_img, table) ){
		return 1; /* then return the error flag true */
	}

	return 0; /* return the error flag faulse */	
}


int is_data_applied(char *val_str, int num_of_line, int *dc, data_word **data_img, sym_table *table){
	int val; /* the integer value from the string value */
	data_word *w_data; /* the data word to set */
	unsigned int mask; /*take the first 14 bits (from the right)*/

	/* if the data input is not valid for an integer or for a symbol name */
	if( (!is_num_valid(val_str)) && (!is_legal_symbol_name(val_str)) ){
		printf("error: unvalid data input in the data sensence in line %d\n",num_of_line);
		return 0; /* return not valid */
	}

	/* if val string not contain an integer but could be a defined symbol */
	else if( (!is_num_valid(val_str)) && (is_legal_symbol_name(val_str)) ){

		/* then search for a symbol name with the val string with type mdefine in the symbol table */
		sym_table constant  = search_symbol(*table, val_str);

		/* if there is no symbol name as the val string or there is symbol but the attribute not mdefine */
		if((constant == NULL) || (constant->attribute != mdefine) ){
			printf("error: invalid data input in the data sensence in line %d\n",num_of_line); 
			return 0; /* return not valid */
		}

		/* else set the constant value as the integer input */
		val = constant->val;
	}

	/* if the val string contain an integer */
	else if(is_num_valid(val_str)){
		val = atoi(val_str); /* set the integer value as the integer input */
	}

	/* condition if the val cause over flow */
	if(is_data_overflow(val,num_of_line)){
		return 0; /* return not valid */
	}
		
	w_data = (data_word*)malloc_and_check(sizeof(data_word));
	mask = 16383;  /* 16383 = 1111 1111 1111 11 (in bin) */
	w_data->data_val = val&mask; /*take the first 14 bits (from the right)*/
	data_img[*dc] = w_data; /* add the input data value into the data image */
	++(*dc); /* increase the data counter */

	return 1; /* return valid */
}

int define_sentence(char *deff_const, int num_of_line, sym_table *table){

	char cons_name[MAX_CHAR_LINE] = {0}; /* string that contains the constant name */
	char cons_val[MAX_CHAR_LINE] = {0}; /* string that contains the constant value */
	int val; /* the integer value of the constant argument */
	char *eqp; /* pointer to equal char */
	int equal_char_pos;/* the position of the equal char */


	/* if there is no text word after .define in the source file */
	if( deff_const == NULL ){
		printf("error: empty sentence of define constant in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	eqp = strchr(deff_const, '=');


	/*------------sets the cons_name------------*/

	/* in case the constant name NOT include the equal char (=) , examples: .define consName =88 or .define consName = 88 */
	if(eqp == NULL){

		strcpy(cons_name, deff_const); /* sets the current text word as constant name  */
		deff_const = strtok(NULL, " \t\f\r\n");

		/* if there is no text word after the constant name in the source file */
		if( deff_const == NULL ){
			printf("error: empty sentence of define constant missing value to set the constant in line %d\n", num_of_line);
			return 1;  /* return the error flag true */
		}

		/* if the next text word from the deffenition of the constant not include the equal char */
		if(deff_const[0] != '='){
			printf("error: missing the equal char '=' for define constant in line %d\n", num_of_line);
			return 1;  /* return the error flag true */
		}

		eqp = strchr(deff_const, '='); /* here eqp must contain the equal char */		
	}

	/* in case the constant deffenition include the equal char (=), examples: .define consName=88 or .define consName= 88 or =88 */
	else if(eqp != NULL){

		/* if the first char of the text word after ".define" is equal char */
		if(deff_const[0] == '='){
			printf("error: invalid define sentence, missing constant name argument before '=' in line %d\n", num_of_line);
			return 1; /* return the error flag true */
		}

		equal_char_pos = strcspn(deff_const,"="); /* set the position of the equal char */
		strncpy(cons_name, deff_const, equal_char_pos); /* sets the constant name up to the equal char */
	}



	/*------------sets the cons_val------------*/


	/* in case the equal char pointer and the constant value is with no white space, examples : .define consName=88 or consName =88 */
	if( strlen(eqp) > 1 ){
		eqp++;
		strcpy(cons_val,eqp); /* sets the string of the constant number after the equal char */
	}

	/* condition if the equal char pointer only include only the equal char, examples: .define consName= 88 .define consName = 88 */
	else if(strlen(eqp) == 1){

		deff_const = strtok(NULL, " \t\f\r\n");

		/* if there is no text word after the constant name in the source file */
		if( deff_const == NULL ){
			printf("error: no integer value for the defined constant in line %d\n", num_of_line);
			return 1;  /* return the error flag true */
		}
		strcpy(cons_val,deff_const); /* sets the next text word after the equal char as the string of the constant number */
	}


	/*------------check the valid of the cons_name and the cons_val------------*/
		
	/* if the label name of the constant not legal */
	if( !(is_legal_symbol_name(cons_name)) ){
		printf("error: invalid constant label name in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	/* if the name of the constant already exists in the symbol table */
	if( is_symbol_exsits(*table, cons_name) ){
		printf("error: duplicate symbols names in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	/* if the input number not valid */
	if(!is_num_valid(cons_val)){
		printf("error: invalid input to constant number deffenition in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	deff_const = strtok(NULL, " \t\f\r\n");

	/* check if there is arguments after the constant definition */
	if(deff_const != NULL){
		printf("error: too many arguments in one sentence in line %d\n", num_of_line);
		return 1;  /* return the error flag true */
	}

	/* if we got here theres no error of the constant deffenition */
	val = atoi(cons_val);

	add_symbol(table, cons_name, val, mdefine);

	return 0; /* return the error flag false */
}





