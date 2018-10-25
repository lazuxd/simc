%{
    #include <stdio.h>
    #include <stdlib.h>
    extern int yylex();
    extern void yyerror(const char*);
    extern void yyset_in(FILE*);
    extern int yylineno;
    extern char* yytext;
%}

%start PROGRAM
%token k_let
%token k_in
%token k_end
%token k_integer
%token k_skip
%token k_if
%token k_then
%token k_else
%token k_fi
%token k_while
%token k_do
%token k_read
%token k_write
%token assign
%token number
%token identifier

%%

PROGRAM             : k_let DECL k_in COMMAND_LIST k_end                                {printf("Program parsed\n");}
                    ;

DECL                : k_integer ID_LIST '.'
                    |
                    ;

ID_LIST             : identifier
                    | identifier ',' ID_LIST
                    ;

COMMAND_LIST        : COMMAND ';' COMMAND_LIST
                    |
                    ;

COMMAND             : k_skip
                    | identifier assign E
                    | k_if E k_then COMMAND_LIST k_else COMMAND_LIST k_fi
                    | k_while E k_do COMMAND_LIST k_end
                    | k_read identifier
                    | k_write E
                    ;

E                   : T3
                    | T3 '=' T3
                    | T3 '<' T3
                    | T3 '>' T3
                    ;

T3                  : T2 '+' T3
                    | T2 '-' T3
                    | T2
                    ;

T2                  : T1 '*' T2
                    | T1 '/' T2
                    | T1
                    ;

T1                  : T '^' T1
                    | T
                    ;

T                   : number
                    | identifier
                    | '(' E ')'
                    ;

%%

void yyerror(const char * s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    return yyparse();
}