#ifndef _LABLE_NAME_AND_NUM_VALID_
#define _LABLE_NAME_AND_NUM_VALID_

#include "constants.h"
#include "symbol_table.h"

#include <ctype.h>
#include <string.h>

/* this file contains functions that checks the valid of a symbol name */


/* This function checks if the name of the symbol is legally
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is valid else return 0.
*/
int is_legal_symbol_name(char *symbol_name);


/* This function checks if the name of the symbol is contains only alphabet and digit characters
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is contains only alphabet and digit characters else return 0.
*/
int only_abc_or_dig(char *symbol_name);


/* This function checks if the name of the symbol is a name of a register or operation or fixed sentence
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is NOT a name of register or operartion or fixed sentence else return 0.
*/
int not_fixed_name(char *symbol_name);

/* This function checks if the name of the symbol is a name of a register
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is a name of register else return 0.
*/
int is_register_name(char *symbol_name);

/* This function checks if the name of the symbol is a name of a operation
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is a name of operartion else return 0.
*/
int is_op_name(char *symbol_name);

/* This function checks if the name of the symbol is a name of a fixed sentence
 * @param symbol_name: the name of the symbol
 * return: 1 if the symboll name is a name of fixed sentence else return 0.
*/
int is_fixed_sentence(char *symbol_name);

/* this function checks if the string that contains number is valid
 * @param num: a string that contains number
 * return: 1 if the number is valid else return 0
*/
int is_num_valid(char *num);

#endif
