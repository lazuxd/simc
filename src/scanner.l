%{
    #include "token_classes.h"
%}

%option yylineno

%%

"let"|"in"|"end"|"integer"|"skip"|"if"|"then"|"else"|"fi"|"while"|"do"|"read"|"write"       return KEYWORD;
[0-9]+                                                                                      return NUMBER;
[a-zA-Z]+[0-9]*                                                                             return IDENTIFIER;
[ \t\n]+                                                                                    return WHITESPACE;
[,.;]                                                                                       return PUNCTUATION;
[()]                                                                                        return PARANTHESIS;
":="                                                                                        return ASSIGNMENT;
[+\-*/^]                                                                                     return ARITHMETIC;
[=<>]                                                                                       return COMPARISON;
.                                                                                           return ERROR;

%%

int yywrap(void) {
    return 1;
}