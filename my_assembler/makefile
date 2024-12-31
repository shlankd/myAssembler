
CC = gcc

CFLAGS = -Wall -ansi -pedantic

MY_ASSEMBLER = assembler.o pre_assembler.o macro_data.o functions_with_check.o first_pass.o first_pass_utils.o symbol_table.o lable_name_and_num_valid.o operation.o line_runner.o second_pass.o files_output.o

RM = rm -f

my_assembler: $(MY_ASSEMBLER)
	$(CC) -g $(CFLAGS) $(MY_ASSEMBLER) -o $(@)

assembler.o: assembler.c $(DATA_H)
	$(CC) -c -g $(CFLAGS) assembler.c -o $(@)

pre_assembler.o: pre_assembler.c pre_assembler.h
	$(CC) -c -g $(CFLAGS) pre_assembler.c -o $(@)

macro_data.o: macro_data.c macro_data.h
	$(CC) -c -g $(CFLAGS) macro_data.c -o $(@)

line_runner.o: line_runner.c line_runner.h
	$(CC) -c -g $(CFLAGS) line_runner.c -o $(@)

second_pass.o: second_pass.c second_pass.h
	$(CC) -c -g $(CFLAGS) second_pass.c -o $(@)

functions_with_check.o: functions_with_check.c functions_with_check.h
	$(CC) -c -g $(CFLAGS) functions_with_check.c -o $(@)

first_pass.o: first_pass.c first_pass.h
	$(CC) -c -g $(CFLAGS) first_pass.c -o $(@)

first_pass_utils.o: first_pass_utils.c first_pass_utils.h
	$(CC) -c -g $(CFLAGS) first_pass_utils.c -o $(@)

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) -c -g $(CFLAGS) symbol_table.c -o $(@)

lable_name_and_num_valid.o: lable_name_and_num_valid.c lable_name_and_num_valid.h
	$(CC) -c -g $(CFLAGS) lable_name_and_num_valid.c -o $(@)

operation.o: operation.c operation.h
	$(CC) -c -g $(CFLAGS) operation.c -o $(@)

files_output.o: files_output.c files_output.h
	$(CC) -c -g $(CFLAGS) files_output.c -o $(@)

clean:
	$(RM) $(MY_ASSEMBLER) my_assembler
	
