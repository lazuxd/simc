#include <stdio.h>
#include <stdlib.h>
#include "data_struct.h"
#include "main.h"
#include "y.tab.h"
#include "mips.h"
extern int yylex();
extern void yyerror(const char*);
extern void yyset_in(FILE*);
extern int yylineno;
extern char* yytext;

struct si_node* var_table[TABLE_SIZE];
struct si_node* const_table[TABLE_SIZE];
struct cmd_node* cmd_list;
char* filename;
int mode;
int var_count = 0;
char var_str[VAR_STR_SIZE] = "";
int var_str_next = 0;

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: simc [option] [filename]\noptions:\n\t-i        to interpret the program\n\t-mips        to generate MIPS Assembly code\n");
        return 0;
    }
    if (strcmp(argv[1], "-mips") == 0) mode = MIPS;
    else mode = INTERPRETER;
    filename = argv[2];
    FILE* fp = fopen(filename, "r");
    yyset_in(fp);
    yyparse();
    fclose(fp);

    if (mode == MIPS) {
        int l = strlen(filename) + 1;
        char* asm_filename = (char*) malloc(l);
        strncpy(asm_filename, filename, l);
        strcpy(&asm_filename[l-4], "asm"); 
        FILE* fasm = fopen(asm_filename, "w");

        fprintf(fasm, mips_header, strlen(var_str) == 0 ? "0" : var_str+2, STACK_SIZE);
        mips_cmd_codeGen(cmd_list, fasm);
        fprintf(fasm, mips_footer, "");

        fclose(fasm);
        free(asm_filename);
    } else {
        exec(cmd_list);
    }

    free_resources();
    return 0;
}

void yyerror(const char * s) {
    fprintf(stderr, "Parse error near line %d in file '%s'. Token that generated the error: '%s'\n", yylineno, filename, yytext);
    free_resources();
    exit(1);
}
void free_resources() {
    delete_table(var_table, TABLE_SIZE);
    delete_cmd_list(cmd_list);
}
char* i2s(int n) {
    static char str[30];
    sprintf(str, "%d", n);
    return str;
}
