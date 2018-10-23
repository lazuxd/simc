#define LEXEME_MAX 32
struct token {
    int token_class;
    char lexeme[LEXEME_MAX];
};

int get_nr_of_tokens(char* filename);
struct token* get_tokens(char* filename, int * nr_out);