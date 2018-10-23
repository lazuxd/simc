#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_classes.h"
#include "scanner.h"

static char* token_classes[] = TOKEN_ARR;

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("No filename entered!\n");
        return 0;
    }
    int nr;
    struct token * tokens = get_tokens(argv[1], &nr);
    for (int i = 0; i < nr; i++) {
        printf("%s <-> %s\n", token_classes[tokens[i].token_class-1], tokens[i].lexeme);
    }

    return 0;
}