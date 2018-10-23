#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_classes.h"
#include "scanner.h"

extern int yylex();
extern void yyset_in(FILE*);
extern int yylineno;
extern char* yytext;

static char* token_classes[] = TOKEN_ARR;

int get_nr_of_tokens(char* filename) {
    FILE* f = fopen(filename, "r");
    yyset_in(f);

    int token_class, nr = 0;
    token_class = yylex();
    while (token_class) {
        if (token_class == ERROR) {
            printf("Syntax error near line %d in file %s. Unexpected token: %s\n", yylineno, filename, yytext);
            fclose(f);
            return -1;
        } else if (token_class != WHITESPACE) {
            ++nr;
        }
        token_class = yylex();
    }

    fclose(f);

    return nr;
}

struct token* get_tokens(char* filename, int * nr_out) {
    int nr = get_nr_of_tokens(filename);
    *nr_out = nr;
    if (nr == -1) return NULL;
    struct token * tokens = malloc(nr*sizeof(struct token));

    FILE* f = fopen(filename, "r");
    yyset_in(f);
    int token_class = yylex(), i = 0;
    while (token_class) {
        if (token_class != WHITESPACE) {
            tokens[i].token_class = token_class;
            strncpy(tokens[i++].lexeme, yytext, LEXEME_MAX);
        }
        token_class = yylex();
    }
    fclose(f);

    return tokens;
}


// int main(int argc, char** argv) {
//     FILE * fin;
//     if (argc > 1) {
//         fin = fopen(argv[1], "r");
//     } else {
//         fin = stdin;
//     }
//     yyset_in(fin);
//     int token_class;
//     token_class = yylex();
//     while (token_class) {
//         printf("%s%*c%s\n", token_classes[token_class-1], 10, ' ', yytext);
//         token_class = yylex();
//     }

//     fclose(fin);

//     return 0;
// }