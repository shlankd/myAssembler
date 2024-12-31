
#ifndef _OPERATION_
#define _OPERATION_


#include "constants.h"
#include "symbol_table.h"
#include "lable_name_and_num_valid.h"
#include "operation.h"

#include <stdio.h>
#include <string.h>


/* this function reads the operation sentence from the source file and sets the operands arguments and the amount of operands.  
 * if there is syntax error of the operation sentence like comma char missing between two operands or missing operand before comma char  
 * or too many arguments of operands then returns 1 else returns 0.
 * @param op_sentence: the first token string from the current line that does not contain  white space. 
 * @param num_of_line: the number of the current line of the source file that the operation sentence appears.
 * @param first_operand: pointer to the string that need to be set to the first operand argument (if exists)
 * @param second_operand: pointer to the string that need to be set to the second operand argument (if exists)
 * @param op_counter: pointer to the operands counter
 * return: if there syntax error of the operation sentence returns 1 else returns 0
*/
int set_operands(char *op_sentence, int num_of_line, char **first_operand, char **second_operand, int *op_counter);


/* This function checks if the operands are valid for the operation sentence and sets their address types. 
 * Returns 1 if there is an error else returns 0 and sets the types addresses of the operands.
 * @param opcode: the opcode of the current operation
 * @param num_of_line: the current line number
 * @param first_operand: the first operand argument
 * @param second_operand: the second operand argument
 * @param op_counter: the number of operands
 * @param types_arr: the addresses types array to be set. 
 * 			types_arr[0]= address type of the first operand
 * 			types_arr[1]= address type of the second operand
*/
int is_op_invalid(opcodes opcode, int num_of_line, char *first_operand, char *second_operand, int op_counter, adress_type *types_arr);


/* this function gets an operand string argument and determine the address type of the operand.
 * if there is syntax error of the operand argument then the function returns the address type ERROR for invalid operand
 * @param operand_arg: the operand string argument
 * return: the address type of the operand argument
*/
adress_type get_address_type(char *operand_arg);


/* this function sets the opcode by the string input op_str. the function compare the op_str to every opname 
 * and if the op_str contains opname then set_op gets the opcode of that opname.
 * @param op_str: a string from the input file that contains operation name
 * @param set_op: a pointer to typedef opcodes that need to set
*/
void set_opcode(char *op_str, opcodes *set_op);


/* This function checks if operand type is legal for the operation that appears in the sentence.
 * @param curr_op: the operation code of the current sentence
 * @param  ad_type: the address type of the operand that appears in the current sentence
 * @param is_source_operand: a flag that indicates if the input address type of the operand is source operand or destination operand
 * return: returns 1 if the address type associated to the opcode else returns 0.
*/
int is_legal_address(opcodes curr_op, adress_type ad_type, int is_source_operand);


#endif

