
#ifndef _MACRO_DATA_H_
#define _MACRO_DATA_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functions_with_check.h"

/* this file contains defenitions of macro type and list that contains macro type
 * and declerations of functions that uses for the pre assumbler stage. 
*/

/* define a list of macros */

typedef struct mcr_node * ptr_mcr; /* pointer type to a node of macro list */

/* the type macro contains the name of the macro and the positions in the source file of the starting and ending definition of the macro */
typedef struct mcr_node{
	char *name_mcr; /* the macro name */
	long start_mcr; /* the start position of the macro definition */
	long end_mcr; /* the end position of the macro definition */
	ptr_mcr next_mcr; /* pointer to the next macro node of the list*/
} macro;

/* declerations of functions to the list of macros */

/* add macro node to macro list
 * @param list: a pointer to the list of macros
 * @param macro_name: a string that contains the macro name 
 * @param start: the start position from the source file of the macro definition
 * @param end: the end position from the source file of the macro definition
 */
void add_macro(ptr_mcr *list, char *macro_name, long start, long end);

/* search macro node from the list by the name of the macro 
 * @param list: pointer to the macro list 
 * @param macro_name: a string that contains the macro name
 * return: returns the macro node pointer if exists else returns null
*/
ptr_mcr search_macro_node(ptr_mcr list, char *macro_name); 

/* this function answer if the name of the macro is already exists in the list of macro. 
 * @param list: pointer to the macro list 
 * @param name: a string that contains the macro name
 * return: returns 1 if the name of the macro exists else returns 0
*/
int is_macro_name_exists(ptr_mcr list, char *name);

/* prints the list of macros 
 * @param list: pointer to the macro list
*/
void print_list_macro(ptr_mcr list);

/* free each allocated memory that used for the data of the macro node in the list of macros
 * @param list: pointer to the macro list
*/
void free_macro_list(ptr_mcr *list);

#endif

