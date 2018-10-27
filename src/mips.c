#include "mips.h"

int label_nr = 0;
char mips_header[] = ".data\nvar: .word %s\nstack: .space %d\n.text\n.globl main\n\nmain:\n\tla $s5, stack\n";
char mips_footer[] = "\tli $v0, 10\n\tsyscall\n";

void mips_cmd_codeGen(struct cmd_node* t, FILE* f) {
    if (t == NULL) return;
    switch (t->type) {
        case IF:
            {
                mips_exp_codeGen(t->_if_.exp, f);
                int label_else = label_nr++;
                int label_fi = label_nr++;
                fprintf(f, "\tbeq $zero, $s0, l%d\n", label_else);
                mips_cmd_codeGen(t->_if_.cmd_list1, f);
                fprintf(f, "\tj l%d\nl%d:\n", label_fi, label_else);
                mips_cmd_codeGen(t->_if_.cmd_list2, f);
                fprintf(f, "l%d:\n", label_fi);
            }
            break;
        case WHILE:
            {
                int label_do = label_nr++;
                int label_end = label_nr++;
                fprintf(f, "l%d:\n", label_do);
                mips_exp_codeGen(t->_while_.exp, f);
                fprintf(f, "\tbeq $zero, $s0, l%d\n", label_end);
                mips_cmd_codeGen(t->_while_.cmd_list, f);
                fprintf(f, "\tj l%d\nl%d:\n", label_do, label_end);
            }
            break;
        case ASSIGN:
            {
                mips_exp_codeGen(t->_assign_.exp, f);
                int i;
                get_var(var_table, TABLE_SIZE, t->_assign_.identifier, &i);
                fprintf(f, "\tsw $s0, var+%d\n", 4*i);
            }
            break;
        case READ:
            {
                int i;
                get_var(var_table, TABLE_SIZE, t->_read_.identifier, &i);
                fprintf(f, "\tli $v0, 5\n\tsyscall\n\tsw $v0, var+%d\n", 4*i);
            }
            break;
        case WRITE:
            {
                mips_exp_codeGen(t->_write_.exp, f);
                fprintf(f, "\tadd $a0, $zero, $s0\n\tli $v0, 1\n\tsyscall\n\taddi $a0, $zero, 10\n\taddi $v0, $zero, 11\n\tsyscall\n");
            }
            break;
        case SKIP:
            fprintf(f, "\n");
            break;
    }
    mips_cmd_codeGen(t->next, f);
}
void mips_exp_codeGen(struct exp_node* exp, FILE* f) {
    if (exp->type == NUMBER) {
        int i;
        get_var(var_table, TABLE_SIZE, i2s(exp->num), &i);
        fprintf(f, "\tlw $s0, var+%d\n\tsw $s0, 0($s5)\n\taddi $s5, $s5, 4\n", 4*i);
        return;
    } else if (exp->type == IDENTIFIER) {
        int i;
        get_var(var_table, TABLE_SIZE, exp->id, &i);
        fprintf(f, "\tlw $s0, var+%d\n\tsw $s0, 0($s5)\n\taddi $s5, $s5, 4\n", 4*i);
        return;
    }
    mips_exp_codeGen(exp->l, f);
    mips_exp_codeGen(exp->r, f);
    fprintf(f, "\tlw $s2, -4($s5)\n\tlw $s1, -8($s5)\n\taddi $s5, $s5, -8\n");
    switch (exp->op) {
        case '+':
            {
                fprintf(f, "\tadd $s0, $s1, $s2\n");
            }
            break;
        case '-':
            {
                fprintf(f, "\tsub $s0, $s1, $s2\n");
            }
            break;
        case '*':
            {
                fprintf(f, "\tmul $s0, $s1, $s2\n");
            }
            break;
        case '/':
            {
                fprintf(f, "\tdiv $s1, $s2\n\tmflo $s0\n");
            }
            break;
        case '^':
            {
                int label_do = label_nr++;
                int label_end = label_nr++;
                fprintf(f, "\taddi $s0, $zero, 1\nl%d:\taddi $s2, $s2, -1\n\tblt $s2, $zero, l%d\n\tmul $s0, $s0, $s1\n\tj l%d\nl%d:\n", label_do, label_end, label_do, label_end);
            }
            break;
        case '<':
            {
                fprintf(f, "\tslt $s0, $s1, $s2\n");
            }
            break;
        case '>':
            {
                fprintf(f, "\tslt $s0, $s2, $s1\n");
            }
            break;
        case '=':
            {
                fprintf(f, "\tslt $s0, $s1, $s2\n\tslt $s1, $s2, $s1\n\tor $s0, $s0, $s1\n\taddi $s1, $zero, 1\n\tsub $s0, $s1, $s0\n");
            }
            break;
    }
    fprintf(f, "\tsw $s0, 0($s5)\n\taddi $s5, $s5, 4\n");
}