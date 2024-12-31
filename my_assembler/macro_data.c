

#include "macro_data.h"
#include "functions_with_check.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/* implement of function that add macro to the macro list from the head list */
void add_macro(ptr_mcr *list, char *name, long start, long end){

	ptr_mcr tmp; /* pointer to the macro to add */
	char *tmp_name; /* pointer to the name of macro to add */

	/* create the macro node to add to the list of macros */

	tmp = (ptr_mcr)malloc_and_check(sizeof(macro));/*allocate memory using malloc and check if the allocation succeded for macro*/

	tmp_name = (char*)malloc_and_check(strlen(name)+1);/*allocate memory using malloc and check if the allocation succeded for string*/

	strcpy(tmp_name, name);

	tmp->name_mcr = tmp_name;
	tmp->start_mcr = start;
	tmp->end_mcr = end;
	
	
	/* add the new macro node to the head of the list of macros */
	tmp->next_mcr = *list;
	*list = tmp;
}


/* implements the search of a certain macro by the macro name in the list of macros */
ptr_mcr search_macro_node(ptr_mcr list, char *name){
	
	ptr_mcr p; /* pointer to the list of macros */

	p = list;

	/* loop that runs the macro list until the list is end or until the name of the macro is found */
	while( (p != NULL) && (strcmp(p->name_mcr,name)!= 0) ){
		p = p->next_mcr;
	}
	
	return p;
}

int is_macro_name_exists(ptr_mcr list, char *name){

	ptr_mcr p; /* pointer to the macro list */
	int res; /* the result of the search name of the macro */

	res = 0; /* sets the result false */

	p = search_macro_node(list, name); /* activate the search of the macro by his name */

	/* if the search ended with a macro node then the input symbol name is exists */
	if(p != NULL){
		res = 1; /* sets the result true */
	}
	return res;
}

/* implements the print of the list of macros */
void print_list_macro(ptr_mcr list){

	ptr_mcr p; /* pointer to the list of macros */
	p = list;

	/* loop that runs the list of macros and prints the data of each macro from the list */
	while(p != NULL){
		printf("macro name: %s, macro start: %ld, macro end: %ld\n",p->name_mcr, p->start_mcr, p->end_mcr);
		p = p->next_mcr;
	}
}

/* implements the free allocated memory from the list of macros */
void free_macro_list(ptr_mcr *list){

	ptr_mcr p; /* pointer to the list of macros */

	/* loop that runs the list of macros and free each allocated data that used in the macro node */
	while(*list != NULL){
		p = *list; /* set the pointer to the head list */
		*list = (*list)->next_mcr; /*set the head list to the next macro node in the list */
		free(p->name_mcr); /* free the allocated memory that used for the macro name */
		free(p); /* free the allocated memory that used for the macro node */
	}
}

