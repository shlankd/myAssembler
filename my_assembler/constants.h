
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_


#define MEMORY_SIZE 4096

#define WORD_SIZE 14 /* the size of word in bits  */

#define MAX_IMG_SIZE (MEMORY_SIZE/4) /* the maximum instructions image and data image in the source file (result 1024) */

#define MAX_CHAR_LINE 81 /* the maximum chars in line from the source file */

#define MAX_CHAR_LEN_LABLE 31 /* the maximum chars in label */

#define NUM_OF_REGISTERS 8

#define NUM_OF_OPERATIONS 16

#define NUM_OF_FIXED_SENTENCES 5

/* if the total bits for a word to contain data for immediate address is 12 then the range of imeddiate integer values is:
 * -2^11<=int_val<=2^11-1 =  -2048<=int_val<=2047 */
#define MAX_INT_IMD 2047

#define MIN_INT_IMD -2048

/* if the total bits for a word to contain data is 14 then the range of data integer values is:
 * -2^13<=int_val<=2^13-1 =  -8192<=int_val<=8191 */

#define MAX_INT_DATA 8191

#define MIN_INT_DATA -8192

#define TWO_BITS 2

#define FOUR_BITS 4

#define TWELVE_BITS 12

#define THREE_BITS 3

#define SIX_BITS 6

#define FOUR_TEEN_BITS 14

typedef enum opcodes{

	/* first group */
	mov = 0,
	cmp = 1,
	add = 2,
	sub = 3,
	lea = 6,

	/* second group */
	not = 4,
	clr = 5,
	inc = 7,
	dec = 8,
	jmp = 9,
	bne = 10,
	red = 11,
	prn = 12,
	jsr = 13,

	/* third group */
	rts = 14,
	hlt = 15,

	/* unvalid opname */
	unvalid = -1

}opcodes;

/* structure for createa  list that contains opcodes withe their names */
typedef struct op_list_node{
	opcodes opcode; /* the opcode of certain operation */
	char *opname; /* the opname string of certain operation */
	int src_operand[4]; /* contain the addressing types that that are legal for the source operand of a certain operation */
	int des_operand[4]; /* contain the addressing types that that are legal for the destenation operand of a certain operation */ 
}op_list_node;


/* the code word struction of operation sentence */
typedef struct operation_word{
	unsigned int ARE: 2;
	unsigned int destenation_operand: 2;
	unsigned int source_operand: 2;
	unsigned int opcode: 4;
	unsigned int not_used: 4;
}operation_word;


/* the code word struction for word that contains data from the source file */
typedef struct immediate_word{
	unsigned int ARE: 2;
	unsigned int val: 12;
}immediate_word;

/* the code word struction for word that contains registers operands */
typedef struct reg_word{
	unsigned int ARE: 2;
	unsigned int reg_destenation: 3;
	unsigned int reg_source: 3;
	unsigned int unuse: 6;
}reg_word;

/* the code word struction for word that contains data from the source file */
typedef struct data_word{
	unsigned int data_val: 14;
}data_word;

/* struction for types of words */
typedef struct word{
	operation_word *op;
	immediate_word *imd;
	reg_word *reg;
}word;


/* represents the method of addressing */
typedef enum adress_type{

	IMMEDIATE_ADDRESS = 0,

	DIRECT_ADDRESS = 1,

	FIXED_INDEX_ADDRESS = 2,

	DIRECT_REGISTER_ADDRESS = 3,

	ERROR = -1

}adress_type;

#endif

