%{
    #include "data_struct.h"
    #include "y.tab.h"
    extern char* filename;
    extern void free_resources();
%}

%option yylineno

%%

"let"                       {return k_let;}
"in"                        {return k_in;}
"end"                       {return k_end;}
"integer"                   {return k_integer;}
"skip"                      {return k_skip;}
"if"                        {return k_if;}
"then"                      {return k_then;}
"else"                      {return k_else;}
"fi"                        {return k_fi;}
"while"                     {return k_while;}
"do"                        {return k_do;}
"read"                      {return k_read;}
"write"                     {return k_write;}
":="                        {return assign;}
[+-]?[0-9]+                 {yylval.integer = atoi(yytext); return number;}
[a-zA-Z]+[0-9]*             {strncpy(yylval.string, yytext, strlen(yytext)+1); return identifier;}
","                         {return ',';}
";"                         {return ';';}
"."                         {return '.';}
"="                         {return '=';}
"<"                         {return '<';}
">"                         {return '>';}
"+"                         {return '+';}
"-"                         {return '-';}
"*"                         {return '*';}
"/"                         {return '/';}
"^"                         {return '^';}
"("                         {return '(';}
")"                         {return ')';}
[ \t\n]+                    ;
.                           {
                                fprintf(stderr, "Syntax error near line %d in file '%s'. Unexpected token: %s\n", yylineno, filename, yytext);
                                free_resources();
                                exit(1);
                            }

%%

int yywrap(void) {
    return 1;
}
