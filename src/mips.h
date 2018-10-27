#ifndef __MIPS_H__
#define __MIPS_H__

#include "main.h"
#include "data_struct.h"

extern int label_nr;
extern char mips_header[];
extern char mips_footer[];

void mips_cmd_codeGen(struct cmd_node* t, FILE* f);
void mips_exp_codeGen(struct exp_node* exp, FILE* f);

#endif