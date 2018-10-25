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
    int number;
    char* string;
    struct si_node si_node;
    struct exp_node exp_node;
    struct cmd_node cmd_node;
}
%start PROGRAM
%token k_let k_in k_end k_integer
%token k_if k_then k_else k_fi
%token k_while k_do
%token k_read k_write
%token k_skip
%token assign
%token <number> number
%token <string> identifier
%type <exp_node*> E T3 T2 T1 T
%type <cmd_node*> COMMAND COMMAND_LIST

%%

PROGRAM             : k_let DECL k_in COMMAND_LIST k_end                                {cmd_list = $4;}
                    ;

DECL                : k_integer ID_LIST '.'                                             {;}
                    | /*empty*/                                                         {;}
                    ;

ID_LIST             : identifier                                                        {add_var(var_table, TABLE_SIZE, $1, 0);}
                    | identifier ',' ID_LIST                                            {add_var(var_table, TABLE_SIZE, $1, 0);}
                    ;

COMMAND_LIST        : COMMAND ';' COMMAND_LIST                                          {$1->next = $3; $$ = $1;}
                    | /*empty*/                                                         {$$ = new_skip();}
                    ;

COMMAND             : k_if E k_then COMMAND_LIST k_else COMMAND_LIST k_fi               {$$ = new_if($2, $4, $6);}
                    | k_while E k_do COMMAND_LIST k_end                                 {$$ = new_while($2, $4);}
                    | identifier assign E                                               {$$ = new_assign($1, $3);}
                    | k_read identifier                                                 {$$ = new_read($2);}
                    | k_write E                                                         {$$ = new_write($2);}
                    | k_skip                                                            {$$ = new_skip();}
                    ;

E                   : T3                                                                {$$ = $1;}
                    | T3 '=' T3                                                         {$$ = new_node_from_op('=', $1, $3);}
                    | T3 '<' T3                                                         {$$ = new_node_from_op('<', $1, $3);}
                    | T3 '>' T3                                                         {$$ = new_node_from_op('>', $1, $3);}
                    ;

T3                  : T2 '+' T3                                                         {$$ = new_node_from_op('+', $1, $3);}
                    | T2 '-' T3                                                         {$$ = new_node_from_op('-', $1, $3);}
                    | T2                                                                {$$ = $1;}
                    ;

T2                  : T1 '*' T2                                                         {$$ = new_node_from_op('*', $1, $3);}
                    | T1 '/' T2                                                         {$$ = new_node_from_op('/', $1, $3);}
                    | T1                                                                {$$ = $1;}
                    ;

T1                  : T '^' T1                                                          {$$ = new_node_from_op('^', $1, $3);}
                    | T                                                                 {$$ = $1;}
                    ;

T                   : number                                                            {$$ = new_node_from_num($1);}
                    | identifier                                                        {$$ = new_node_from_id($1);}
                    | '(' E ')'                                                         {$$ = $2;}
                    ;

%%

void yyerror(const char * s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    return yyparse();
}