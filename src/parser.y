%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "data_struct.h"
    #include "main.h"
    extern int yylex();
    extern void yyerror(const char*);
    extern void yyset_in(FILE*);
    extern int yylineno;
    extern char* yytext;
%}

%union {
    int integer;
    char string[50];
    struct si_node* p_si_node;
    struct exp_node* p_exp_node;
    struct cmd_node* p_cmd_node;
}
%start PROGRAM
%token k_let k_in k_end k_integer
%token k_if k_then k_else k_fi
%token k_while k_do
%token k_read k_write
%token k_skip
%token assign
%token <integer> number
%token <string> identifier
%type <p_exp_node> E T3 T2 T1 T
%type <p_cmd_node> COMMAND COMMAND_LIST

%%

PROGRAM             : k_let DECL k_in COMMAND_LIST k_end                                {cmd_list = $4;}
                    ;

DECL                : k_integer ID_LIST '.'                                             {;}
                    | /*empty*/                                                         {;}
                    ;

ID_LIST             : identifier                                                        {
                                                                                            add_var(var_table, TABLE_SIZE, $1, mode == INTERPRETER ? 0 : var_count++);
                                                                                            if (mode != INTERPRETER) {
                                                                                                strcpy(var_str+var_str_next, ", 0");
                                                                                                var_str_next += 3;
                                                                                            }
                                                                                        }
                    | identifier ',' ID_LIST                                            {
                                                                                            add_var(var_table, TABLE_SIZE, $1, mode == INTERPRETER ? 0 : var_count++);
                                                                                            if (mode != INTERPRETER) {
                                                                                                strcpy(var_str+var_str_next, ", 0");
                                                                                                var_str_next += 3;
                                                                                            }
                                                                                        }
                    ;

COMMAND_LIST        : COMMAND ';' COMMAND_LIST                                          {$1->next = $3; $$ = $1;}
                    | /*empty*/                                                         {$$ = new_skip();}
                    ;

COMMAND             : k_if E k_then COMMAND_LIST k_else COMMAND_LIST k_fi               {$$ = new_if($2, $4, $6);}
                    | k_while E k_do COMMAND_LIST k_end                                 {$$ = new_while($2, $4);}
                    | identifier assign E                                               {   
                                                                                            if (!exist_var(var_table, TABLE_SIZE, $1)) {
                                                                                                fprintf(stderr, "Semantic error near line %d in file '%s'. A value is assigned to '%s' but this variable is not declared.\n", yylineno, filename, $1);
                                                                                                free_resources();
                                                                                                exit(1);
                                                                                            }
                                                                                            $$ = new_assign($1, $3);
                                                                                        }
                    | k_read identifier                                                 {
                                                                                            if (!exist_var(var_table, TABLE_SIZE, $2)) {
                                                                                                fprintf(stderr, "Semantic error near line %d in file '%s'. A value is assigned to '%s' but this variable is not declared.\n", yylineno, filename, $2);
                                                                                                free_resources();
                                                                                                exit(1);
                                                                                            }
                                                                                            $$ = new_read($2);
                                                                                        }
                    | k_write E                                                         {$$ = new_write($2);}
                    | k_skip                                                            {$$ = new_skip();}
                    ;

E                   : T3                                                                {$$ = $1;}
                    | T3 '=' T3                                                         {$$ = new_node_from_op('=', $1, $3);}
                    | T3 '<' T3                                                         {$$ = new_node_from_op('<', $1, $3);}
                    | T3 '>' T3                                                         {$$ = new_node_from_op('>', $1, $3);}
                    ;

T3                  : T3 '+' T2                                                         {$$ = new_node_from_op('+', $1, $3);}
                    | T3 '-' T2                                                         {$$ = new_node_from_op('-', $1, $3);}
                    | T2                                                                {$$ = $1;}
                    ;

T2                  : T2 '*' T1                                                         {$$ = new_node_from_op('*', $1, $3);}
                    | T2 '/' T1                                                         {$$ = new_node_from_op('/', $1, $3);}
                    | T1                                                                {$$ = $1;}
                    ;

T1                  : T1 '^' T                                                          {$$ = new_node_from_op('^', $1, $3);}
                    | T                                                                 {$$ = $1;}
                    ;

T                   : number                                                            {
                                                                                            $$ = new_node_from_num($1);
                                                                                            if (mode != INTERPRETER) {
                                                                                                char * s = i2s($1);
                                                                                                if (!exist_var(var_table, TABLE_SIZE, s)) {
                                                                                                    add_var(var_table, TABLE_SIZE, s, var_count++);
                                                                                                    var_str[var_str_next++] = ',';
                                                                                                    var_str[var_str_next++] = ' ';
                                                                                                    strcpy(var_str+var_str_next, s);
                                                                                                    var_str_next += strlen(s);
                                                                                                }
                                                                                            }
                                                                                        }
                    | identifier                                                        {
                                                                                            if (!exist_var(var_table, TABLE_SIZE, $1)) {
                                                                                                fprintf(stderr, "Semantic error near line %d in file '%s'. Variable '%s' is used but not declared.\n", yylineno, filename, $1);
                                                                                                free_resources();
                                                                                                exit(1);
                                                                                            }
                                                                                            $$ = new_node_from_id($1);
                                                                                        }
                    | '(' E ')'                                                         {$$ = $2;}
                    ;

%%
