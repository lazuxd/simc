#include <stdio.h>
#include <stdlib.h>
#include "data_struct.h"
#include "main.h"
#include "y.tab.h"
extern int yylex();
extern void yyerror(const char*);
extern void yyset_in(FILE*);
extern int yylineno;
extern char* yytext;

struct si_node* var_table[TABLE_SIZE];
struct cmd_node* cmd_list;
char* filename;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No filename given.\n");
        return 0;
    }
    filename = argv[1];
    FILE* fp = fopen(filename, "r");
    yyset_in(fp);
    yyparse();
    fclose(fp);

    exec(cmd_list);

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
