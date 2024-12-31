

#include "constants.h"
#include "symbol_table.h"
#include "functions_with_check.h"

#include "files_output.h"

#include <stdio.h>
#include <string.h>


int output_files(char *file_name, int i_size, int d_size, word **instruc_img, data_word **data_img, sym_table table, sym_table ext_list){

	int err_flag; /* an error flag only to indicate if there is an error of open file */
	int size_file_name; /* contains the origenal size of the input file name argument */

	file_name[strlen(file_name)-3] = '\0'; /* erase the endig ".am" that in the string file name */
	err_flag = 0; /* sets the error flag to false */
	size_file_name = strlen(file_name); /* sets the size of the origenal size of the input file name argument  */

	err_flag = create_ob_file(file_name, i_size, d_size, instruc_img, data_img); /* create the object file */

	/* if there is failure of open file */
	if(err_flag){
		return err_flag;
	}

	file_name[strlen(file_name)-3] = '\0'; /* erase the endig ".ob" that in the string file name */

	err_flag = create_ent_file(file_name, table); /* create entries file */

	/* if there is failure of open file */
	if(err_flag){
		return err_flag;
	}

	/* check if the file name string changed due to the creates of entries file */
	if(strlen(file_name) > size_file_name){
		file_name[strlen(file_name)-4] = '\0'; /* erase the endig ".ent" that in the string file name (if exists) */
	}
	

	err_flag = create_ext_file(file_name, table, ext_list); /* create externals file */

	return err_flag; /* returns the error flag value */
}


int create_ob_file(char *file_name, int i_size, int d_size, word **instruc_img, data_word **data_img){

	FILE *ob_file;
	
	strcat(file_name, ".ob");

	ob_file = fopen_and_check(file_name, "w+"); /* create the output file of object */

	/* condition if the file of object is not open */
	if(ob_file == NULL){
		return 1; /* return the value of true to indicate an error */
	}

	fprintf(ob_file, "\t%d %d\n", i_size, d_size);
	print_ins_image(ob_file, instruc_img, i_size);
	print_data_image(ob_file, data_img, d_size, i_size);

	return 0;

}


int create_ext_file(char *file_name, sym_table table, sym_table ext_list){

	FILE *ext_file;
	
	strcat(file_name, ".ext");

	ext_file = fopen_and_check(file_name, "w+"); /* create the output file of externals */

	/* condition if the file of externals is not open */
	if(ext_file == NULL){
		return 1; /* return the value of true to indicate an error */
	}

	/* check if there is an external symbol */
	if(!is_external(table)){
		return 0; /* returns faulse value for no error indicate */
	}

	while(ext_list != NULL){

		/* condition if the external symbol is the last one in the symbol external list */
		if(ext_list->next == NULL){
			fprintf(ext_file, "%s\t%d", ext_list->name, ext_list->val); /* prints without '\n' */
			break;
		}
		
		/* else print regular external symbol from the external list symbols */
		fprintf(ext_file, "%s\t%d\n", ext_list->name, ext_list->val); /* prints with '\n' */

		ext_list = ext_list->next;
	}

	return 0; /* returns faulse value for no error indicate */
	
}


int create_ent_file(char *file_name, sym_table table){

	FILE *ent_file;

	sym_table ent_sym; /* pointer to symbol table */
	
	strcat(file_name, ".ent");

	ent_file = fopen_and_check(file_name, "w+"); /* create the output file of entries */

	/* condition if the file of entries is not open */
	if(ent_file == NULL){
		return 1; /* return the value of true to indicate an error */
	}

	/* check if there is an entries symbol */
	if(!is_entry(table)){
		return 0; /* returns faulse value for no error indicate */
	}

	ent_sym = table;

	while(ent_sym != NULL){

		/* condition if the entry symbol is the last one in the symbol table */
		if(ent_sym->attribute == entry && !is_entry(ent_sym->next)){
			fprintf(ent_file, "%s\t%d", ent_sym->name, ent_sym->val);  /* prints without'\n' */
			break;
		}

		/* condition to print regular entry symbol */
		else if(ent_sym->attribute == entry){
			fprintf(ent_file, "%s\t%d\n", ent_sym->name, ent_sym->val);  /* prints with '\n' */
		}

		ent_sym = ent_sym->next;
	}
	return 0; /* returns faulse value for no error indicate */
	
}



/* ----------------------------------------------printers functions ---------------------------------------------- */

void print_bin_reg_word(FILE *ob, reg_word x){ /* -----------------------reg_word printer-------------------------- */
	print_bin_unuse(ob, x.unuse);
	print_bin_reg(ob, x.reg_source, x.reg_destenation);
	print_bin_ARE(ob, x.ARE);
}

void print_bin_unuse(FILE *ob, unsigned int x){ 
	reg_word mask;
	int i; /* string iterator */
	char trans_bits[SIX_BITS]; /* string that transfer two bits to base 4 */
	mask.unuse = 1 << (SIX_BITS - 1);
	i=0;

	memset(trans_bits, 0, SIX_BITS);

	while(mask.unuse){

		/*if(i == 2){
			print_base4(ob, trans_bits);
			memset(trans_bits, 0, 3);*/ /* reset the string that transfer bits to base 4  */
			/*i = 0;
		}*/

		if( (x&mask.unuse) == 0 ){
			trans_bits[i] = '0';
		}

		else{
			trans_bits[i] = '1';
		}

		++i;
		mask.unuse >>= 1;
	}

	for(i=1; i<SIX_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}
}

void print_bin_reg(FILE *ob, unsigned int source, unsigned int dest){
	reg_word mask;
	int i; /* string iterator */
	/*int n_bits = 3;*/
	char trans_bits[(THREE_BITS*2)]; /* string that transfer two bits to base 4 */
	mask.reg_source = 1 << (THREE_BITS - 1);
	i=0;

	memset(trans_bits, 0, (THREE_BITS*2));

	/* loop for print the register source */
	while(mask.reg_source){
		if( (source&mask.reg_source) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.reg_source >>= 1;
	}

	mask.reg_destenation = 1 << (THREE_BITS - 1);	

	/* loop for print the register destenation */
	while(mask.reg_destenation){
		if( (dest&mask.reg_destenation) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.reg_destenation >>= 1;
	}

	for(i=1; i<(THREE_BITS*2); i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}

}


void print_bin_imd_word(FILE *ob, immediate_word x){ /* -----------------------immediate_word printer-------------------------- */
	print_bin_imd_val(ob, x.val);
	print_bin_ARE(ob, x.ARE);
		
}

void print_bin_imd_val(FILE *ob, unsigned int x){
	immediate_word mask;
	int i; /* string iterator */
	char trans_bits[TWELVE_BITS]; /* string that transfer two bits to base 4 */
	mask.val = 1 << (TWELVE_BITS - 1);
	memset(trans_bits, 0, TWELVE_BITS);
	i=0;
	while(mask.val){
		/*if(i == 2){
			print_base4(ob, trans_bits);*/
			/*memset(trans_bits, 0, 3);*/ /* reset the string that transfer bits to base 4  */
			/*i = 0;
		}*/
		if( (x&mask.val) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.val >>= 1;
	}

	for(i=1; i<TWELVE_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}	
}

void print_bin_data_word(FILE *ob, data_word x){ /* -----------------------data_word printer-------------------------- */
	print_bin_data_val(ob, x.data_val);		
}

void print_bin_data_val(FILE *ob, unsigned int x){
	data_word mask;
	int i; /* string iterator */
	char trans_bits[FOUR_TEEN_BITS]; /* string that transfer two bits to base 4 */	
	mask.data_val = 1 << (FOUR_TEEN_BITS - 1);
	memset(trans_bits, 0, FOUR_TEEN_BITS);
	i=0;
	while(mask.data_val){

		if( (x&mask.data_val) == 0 ){
			trans_bits[i] = '0';
		}

		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.data_val >>= 1;
	}

	for(i=1; i<FOUR_TEEN_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}
}

void print_bin_op_word(FILE *ob, operation_word x){ /* -----------------------operation_word printer-------------------------- */
	
	print_bin_not_used(ob, x.not_used);
	print_bin_opcode(ob, x.opcode);
	print_bin_ARE(ob, x.source_operand);
	print_bin_ARE(ob, x.destenation_operand);
	print_bin_ARE(ob, x.ARE);
		
}

void print_bin_ARE(FILE *ob, unsigned int x){
	operation_word mask;
	int i; /* string iterator */
	char trans_bits[TWO_BITS]; /* string that transfer two bits to base 4 */	
	mask.ARE = 1 << (TWO_BITS-1); /* sets the mask with only one bit on (the rest of the bits are off) and shift it from LSB to MSB  */
	memset(trans_bits, 0, TWO_BITS);
	i=0;
	while(mask.ARE){
		if( (x&mask.ARE) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.ARE >>= 1;
	}

	for(i=1; i<TWO_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}
	
}

void print_bin_opcode(FILE *ob, unsigned int x){
	operation_word mask;
	int i; /* string iterator */
	char trans_bits[FOUR_BITS]; /* string that transfer two bits to base 4 */		
	mask.opcode = 1 << (FOUR_BITS - 1);
	i=0;
	memset(trans_bits, 0, FOUR_BITS);
	while(mask.opcode){

		if( (x&mask.opcode) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.opcode >>= 1;
	}

	for(i=1; i<FOUR_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}	
}

void print_bin_not_used(FILE *ob, unsigned int x){
	operation_word mask;
	int i; /* string iterator */	
	char trans_bits[FOUR_BITS]; /* string that transfer two bits to base 4 */
	mask.not_used = 1 << (FOUR_BITS - 1);
	i=0;
	while(mask.not_used){

		if( (x&mask.not_used) == 0 ){
			trans_bits[i] = '0';
		}
		else{
			trans_bits[i] = '1';
		}
		++i;
		mask.not_used >>= 1;
	}

	for(i=1; i<FOUR_BITS; i=i+2){

		if(trans_bits[i-1] == '0' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '*');
		}

		else if(trans_bits[i-1] == '0' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '#');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '0'){
			fprintf(ob, "%c", '%');
		}

		else if(trans_bits[i-1] == '1' && trans_bits[i] == '1'){
			fprintf(ob, "%c", '!');
		}
	}	
}

void print_ins_image(FILE *ob, word **instruc_img, int size){
	int i;
	for(i=0; i<size; i++){
		if(instruc_img[i] != NULL){

			if(instruc_img[i]->op != NULL){
				fprintf(ob, "%d\t",i+100);
				print_bin_op_word(ob, *instruc_img[i]->op);
				fprintf(ob, "\n");
			}

			else if(instruc_img[i]->imd != NULL){
				fprintf(ob, "%d\t",i+100);
				print_bin_imd_word(ob, *instruc_img[i]->imd);
				fprintf(ob, "\n");
			}

			else if(instruc_img[i]->reg != NULL){
				fprintf(ob, "%d\t",i+100);
				print_bin_reg_word(ob, *instruc_img[i]->reg);
				fprintf(ob, "\n");
			}
		}
	}
}

void print_data_image(FILE *ob, data_word **data_img, int size, int ic_size){
	int i;
	for(i=0; i<size; i++){
		fprintf(ob, "%d\t",i+100+ic_size);
		print_bin_data_word(ob, *data_img[i]);
		if(i != size-1){
			fprintf(ob, "\n");
		}
	}
}

