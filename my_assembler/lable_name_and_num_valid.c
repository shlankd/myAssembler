
#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"

#include <ctype.h>
#include <string.h>

int is_legal_symbol_name(char *symbol_name){
	int result = 0;
	if( symbol_name != NULL && isalpha(symbol_name[0]) && strlen(symbol_name)<= MAX_CHAR_LEN_LABLE ){

		if(only_abc_or_dig(symbol_name) && not_fixed_name(symbol_name) ){
				result = 1;
		}
	}
	return result;
}


int only_abc_or_dig(char *symbol_name){
	int i;
	for(i=0; i<strlen(symbol_name); i++){
		if( !isalnum(symbol_name[i]) ){
			return 0;
		}
	}
	return 1;
}


int not_fixed_name(char *symbol_name){
	if( !(is_register_name(symbol_name)) && !(is_op_name(symbol_name)) && !(is_fixed_sentence(symbol_name)) ){
		return 1;
	}
	return 0;	
}

int is_register_name(char *symbol_name){

	int i;

	char *registers[NUM_OF_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

	for(i=0; i<NUM_OF_REGISTERS; i++){
		if( strcmp(symbol_name, registers[i]) == 0 ){
			return 1;
		}
	}
	return 0;
}


int is_op_name(char *symbol_name){

	int i;

	char *op_name[NUM_OF_OPERATIONS] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red" ,"prn", "jsr", "rts", "hlt"};

	for(i=0; i<NUM_OF_OPERATIONS; i++){
		if( strcmp(symbol_name, op_name[i]) == 0 ){
			return 1;
		}
	}
	return 0;
}

int is_fixed_sentence(char *symbol_name){

	int i;

	char *fixed_sentences[NUM_OF_FIXED_SENTENCES] = {".data", ".string", ".entry", ".extern", ".define"};

	for(i=0; i<NUM_OF_FIXED_SENTENCES; i++){
		if( strcmp(symbol_name, fixed_sentences[i]) == 0 ){
			return 1;
		}
	}
	return 0;
}

int is_num_valid(char *num){

	int i; /* string iterator */
	int start_index = 0; /* the start index of the number */
	
	if( (num[0] == '+') || (num[0] == '-') ){
		start_index = 1;
	}

	for(i=start_index; i<strlen(num); i++){
		if( !isdigit(num[i]) ){
			return 0;
		}
	}
	return 1;
}
