
#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

#include "constants.h"
#include "functions_with_check.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* symbol attributes */
typedef enum symbol_attribute{
	data,
	mdefine,
	external,
	entry,
	code
}symbol_attribute;

typedef struct symbol_node * sym_table; /* pointer to the symbol table */


typedef struct symbol_node{
	char *name; /* the name of the symbol */
	int val; /* the address of the symbol */
	symbol_attribute attribute; /* symbol attribute */
	sym_table next; /* the next symbol in the table */
} symbol;


/* this function adds a symbol to the symbol table
 * @param table: pointer to the symbol table
 * @param name: the symbol name
 * @param val: the addressing value in decimal
 * @param attribute: the attribute of the added symbol
*/
void add_symbol(sym_table *table, char *name, int val, symbol_attribute attribute);


/* this function runs through the symbol table and each symbol with the same attribute of the input parameter (attribute)
 * the function increas the value of the symbol by the value of the input int attributeval_to_add.
 * @param val_to_add: the value in decimal to add to each symbol with the attribute
 * @param input_attribute: the attribute symbol that in the table that need to increase their vals 
*/
void update_sym_vals(sym_table table, int val_to_add, symbol_attribute input_attribute);

/* this function search a symbol by his name from the table of symbols 
 * @param table: pointer to the symbol table 
 * @param symbol_name: a string that contains the symbol name
 * return: returns the symbol node pointer if exists else returns null
*/
sym_table search_symbol(sym_table table, char *symbol_name);


/* this function answer if the name of the symbol is already exits in the table 
 * @param table: pointer to the symbol table 
 * @param symbol_name: a string that contains the symbol name
 * return: returns 1 if the symbol exists else returns 0
*/
int is_symbol_exsits(sym_table table, char *symbol_name);


/* this function sets the attribute of the given label name with the attribute entry. this function called in the second pass.
 * @param table: pointer to the symbol table
 * @param label_name: string that contains the label name to set the attribute entry
 * @param line_n: the current line number of the source file to indicate an error if occure
 * return: returns 1 if there is no error of the given label name, else returns 0
*/
int is_set_label_entry(sym_table *table, char *label_name, int line_n);


/* free each allocated memory that used for the data of the symbol node in the symbol table
 * @param list: pointer to the symbol table
*/
void free_table(sym_table *table);

/* this function answer if there is an entry symbol or not
 * @param table: the symbol table
 * return: if there is at least one entry symbol returns 1, else returns 0.
*/
int is_entry(sym_table table);

/* this function answer if there is an external symbol or not
 * @param table: the symbol table
 * return: if there is at least one external symbol returns 1, else returns 0.
*/
int is_external(sym_table table);


void print_symbol_table(sym_table);

#endif
