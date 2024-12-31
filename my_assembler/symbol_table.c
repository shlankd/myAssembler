

#include "constants.h"
#include "functions_with_check.h"
#include "symbol_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


void add_symbol(sym_table *table, char *name, int val, symbol_attribute attribute){

	char *tmp_name; /* the string name of the added symbol */
	sym_table new_symbol; /* pointer to the new symbol to add */
	sym_table p; /* pointer to the symbol table */

	/* allocate memory using malloc and check if the allocation succeded for string */
	tmp_name = (char*)malloc_and_check(strlen(name)+1);

	/* allocate memory using malloc and check if the allocation succeded for symbol */
	new_symbol = (sym_table)malloc_and_check(sizeof(symbol));

	strcpy(tmp_name, name); /* copy the name symbol into tmp_name */

	/* fill the fields of symbol by the input parameters */
	new_symbol->name = tmp_name;
	new_symbol->val = val;
	new_symbol->attribute = attribute;

	/* insert the new symbol to the symbol table */
	p = *table;

	/* condition for add the new symbol to the head when the symbol table is empty */ 
	if(p == NULL){
		*table = new_symbol;
		new_symbol->next = p;
	}

	else{
		/* set p to point the last symbol table */
		while( (p->next != NULL) ){
			p = p->next;
		}
		/* the loop ends with p->next = NULL */

		new_symbol->next = p->next; /* sets the next field of the new symbol node to point to NULL */
		p->next = new_symbol;
	}
}

void update_sym_vals(sym_table table, int val_to_add, symbol_attribute input_attribute ){
	sym_table p; /* pointer to the symbol table */
	p = table;

	while(p != NULL){
		if(p->attribute == input_attribute){
			p->val = p->val + val_to_add;
		}
		p = p->next;
	}
}


sym_table search_symbol(sym_table table, char *symbol_name){

	sym_table p; /* pointer to the symbol table */
	p = table;

	/* loop that runs the symbol table until the end of the table or until the name of the symbol is found */
	while( (p != NULL) && (strcmp(p->name,symbol_name)!= 0) ){
		p = p->next;
	}

	return p;
}


int is_symbol_exsits(sym_table table, char *symbol_name){

	sym_table p; /* pointer to the symbol with the name of the input parameter (symbo_name) */
	int res; /* the result of the exits of the symbol in the table */

	res = 0; /* sets the result false */
	p = search_symbol(table, symbol_name); /* activate the search of the symbol by his name */

	/* if the search ended with a symbol then the input symbol name is already exists */
	if(p != NULL){
		res = 1; /* sets the result true */
	}

	return res;
}

int is_set_label_entry(sym_table *table, char *label_name, int line_n){

	sym_table sym_name_ent; /* label name to define the attribute entry */

	/* 1. need to check if there is label name with attribute extern if so indicate an error */

	/* search the label name of the operand argument */
	sym_name_ent = search_symbol(*table, label_name);

	/* if the label name of the operand argument does not exists in the symbol table */
	if(sym_name_ent == NULL){
		printf("error: the operand label name argument that in the entry sentence is not declared at line %d\n", line_n);
		return 0; /* return an error */
	}

	/* if the label name of the operand argument is defined as external */
	if(sym_name_ent->attribute == external){
		printf("error: the operand label name argument that in the entry sentence is defined as external at line %d\n", line_n);
		return 0; /* return an error */
	}

	/* if the label name exists in the symbol table and it is not with attribute external */
	sym_name_ent->attribute = entry; /* then sets the attribute of the given symbol to entry */

	return 1; /* return 1 for indicate that the symbol was set with entry attribute with no errors */
}


void free_table(sym_table *table){

	sym_table p; /* pointer to the list of macros */

	/* loop that runs the table of symbols and free each allocated data that used in the symbol node table*/
	while(*table != NULL){
		p = *table; /* set the pointer to the head table */
		*table = (*table)->next; /*set the head table to the next symbol element */
		free(p->name); /* free the allocated memory that used for the symbol name */
		free(p); /* free the allocated memory that used for the symbol node table */
	}
}


int is_entry(sym_table table){
	sym_table p; /* pointer to the symbol table */
	p = table;

	/* loop that runs the symbol table until the end of the table or until an entry symbol is found */
	while( (p != NULL) && (p->attribute != entry) ){
		p = p->next;
	}

	if(p != NULL){
		return 1;
	}

	return 0;
}


int is_external(sym_table table){
	sym_table p; /* pointer to the symbol table */
	p = table;

	/* loop that runs the symbol table until the end of the table or until an external symbol is found */
	while( (p != NULL) && (p->attribute != external) ){
		p = p->next;
	}

	if(p != NULL){
		return 1;
	}

	return 0;
}

void print_symbol_table(sym_table table){
	sym_table p; /* pointer to the list of macros */
	p = table;

	printf("symbol\t\tattribute\t\tvalue\n\n");

	/* loop that runs the list of macros and prints the data of each macro from the list */
	while(p != NULL){
		if(p->attribute == data){
			printf("%s\t\tdata\t\t%d\n", p->name, p->val);
		}
		else if(p->attribute == mdefine){
			printf("%s\t\tmdefine\t\t%d\n", p->name, p->val);
		}
		else if(p->attribute == external){
			printf("%s\t\texternal\t\t%d\n", p->name, p->val);
		}
		else if(p->attribute == code){
			printf("%s\t\tcode\t\t%d\n", p->name, p->val);
		}
		else if(p->attribute == entry){
			printf("%s\t\tentry\t\t%d\n", p->name, p->val);
		}
		p = p->next;
	}
}





