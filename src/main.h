#ifndef __MAIN_H__
#define __MAIN_H__

#include "data_struct.h"
#define STACK_SIZE 512
#define VAR_STR_SIZE 256
#define TABLE_SIZE 100
#define INTERPRETER 30
#define MIPS 31

extern struct si_node* var_table[TABLE_SIZE];
extern int var_count;
extern struct cmd_node* cmd_list;
extern char* filename;
extern void free_resources();
extern int mode;
extern char* i2s(int n);
extern char var_str[VAR_STR_SIZE];
extern int var_str_next;

#endif