%{
    #include "y.tab.h"
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
[0-9]+                      {yylval.number = atoi(yytext); return number;}
[a-zA-Z]+[0-9]*             {yylval.string = yytext; return identifier;}
[ \t\n]+                    ;
.                           {
                                printf("Syntax error near line %d. Unexpected token: %s\n", yylineno, yytext);
                                exit(1);
                            }

%%

int yywrap(void) {
    return 1;
}
