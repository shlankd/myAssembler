

#ifndef _FILES_OUTPUT_
#define _FILES_OUTPUT_

#include "constants.h"
#include "symbol_table.h"
#include "functions_with_check.h"

#include "files_output.h"

#include <stdio.h>
#include <string.h>

/* this function creates the output files of the assembler
 * @param file_name: contain the file name text
 * @param i_size: the size of instruction image array
 * @param d_size: the size of data image array
 * @param instruc_img: the instruction image array
 * @param data_img: the data image array
 * @param table: the symbol table
 * @param ext_list: pointer to list of referred externals symbols
 * return: if occur an error returns 1 else returns 0
*/
int output_files(char *file_name, int i_size, int d_size, word **instruc_img, data_word **data_img, sym_table table, sym_table ext_list);


/* this function creates the output file of object
 * @param file_name: contain the file name text
 * @param i_size: the size of instruction image array
 * @param d_size: the size of data image array
 * @param instruc_img: the instruction image array
 * @param data_img: the data image array
 * return: if occur an error returns 1 else returns 0
*/
int create_ob_file(char *file_name, int i_size, int d_size, word **instruc_img, data_word **data_img);


/* this function creates the output file of entries
 * @param file_name: contain the file name text
 * @param table: the symbol table
 * return: if occur an error returns 1 else returns 0
*/
int create_ent_file(char *file_name, sym_table table);


/* this function creates the output file of externals
 * @param file_name: contain the file
 * @param table: the symbol table
 * @param ext_list: pointer to list of referred externals symbols name text
 * return: if occur an error returns 1 else returns 0
*/
int create_ext_file(char *file_name, sym_table table, sym_table ext_list);




/* ----------------------------------------------printers functions ---------------------------------------------- */

/* image arrays printers */
void print_ins_image(FILE *ob, word **instruc_img, int size);
void print_data_image(FILE *ob, data_word **data_img, int size, int ic_size);

void print_base4(FILE *ob_file, char *two_bits);

/* printer for reg word */
void print_bin_reg_word(FILE *ob, reg_word x);
void print_bin_unuse(FILE *ob, unsigned int x);
void print_bin_reg(FILE *ob, unsigned int source, unsigned int dest);

/* printer for imd word*/
void print_bin_imd_word(FILE *ob, immediate_word x);
void print_bin_imd_val(FILE *ob, unsigned int x);

/* printer for data word */
void print_bin_data_word(FILE *ob, data_word x);
void print_bin_data_val(FILE *ob, unsigned int x);


/* printer for op word */
void print_bin_op_word(FILE *ob, operation_word x);
void print_bin_ARE(FILE *ob, unsigned int x);
void print_bin_opcode(FILE *ob, unsigned int x);
void print_bin_not_used(FILE *ob, unsigned int x);


#endif
