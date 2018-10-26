#include "data_struct.h"
#define DUPLICATE_VAR "Semantic error near line %d in file '%s'. Variable '%s' declared multiple times.\n"
extern int yylineno;

// Hash table

int num(char c) {
    if (c >= '0' && c <= '9') return c-'0';
    else if (c >= 'a' && c <= 'z') return c-'a'+10;
    else if (c >= 'A' && c <= 'Z') return c-'A'+36;
}
int hash(const char * s, int n) {
    unsigned int h = 0;
    while (*s) {
        h = (h<<6)|num(*s++);
    }
    return h%n;
}
void __new_var(struct si_node* t, char* var_name, int var_num) {
    int l = strlen(var_name)+1;
    t->s = (char*) malloc(l);
    strncpy(t->s, var_name, l);
    t->i = var_num;
    t->next = NULL;
}
int add_var(struct si_node** var_arr, int n, char* var_name, int var_num) {
    int i = hash(var_name, n);
    if (var_arr[i] == NULL) {
        var_arr[i] = (struct si_node *) malloc(sizeof(struct si_node));
        __new_var(var_arr[i], var_name, var_num);
    } else {
        struct si_node * t = var_arr[i];
        if (strncmp(t->s, var_name, strlen(var_name)) == 0) {
            fprintf(stderr, DUPLICATE_VAR, yylineno, filename, var_name);
            free_resources();
            exit(EXIT_FAILURE);
            return -1;
        }
        while (t->next != NULL) {
            t = t->next;
            if (strncmp(t->s, var_name, strlen(var_name)) == 0) {
                fprintf(stderr, DUPLICATE_VAR, yylineno, filename, var_name);
                free_resources();
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        t->next = (struct si_node*) malloc(sizeof(struct si_node));
        __new_var(t->next, var_name, var_num);
    }
    return 0;
}
int update_var(struct si_node** var_arr, int n, char* var_name, int var_num) {
    int i = hash(var_name, n);
    if (var_arr[i] == NULL) {
        return -1;
    } else {
        struct si_node * t = var_arr[i];
        while (strncmp(t->s, var_name, strlen(var_name)) != 0) {
            if (t->next == NULL) return -1;
            else t = t->next;
        }
        t->i = var_num;
    }
    return 0;
}
int get_var(struct si_node** var_arr, int n, char* var_name, int* var_num) {
    int i = hash(var_name, n);
    if (var_arr[i] == NULL) {
        return -1;
    } else {
        struct si_node * t = var_arr[i];
        while (strncmp(t->s, var_name, strlen(var_name)) != 0) {
            if (t->next == NULL) return -1;
            else t = t->next;
        }
        *var_num = t->i;
    }
    return 0;
}
int exist_var(struct si_node** var_arr, int n, char* var_name) {
    int i = hash(var_name, n);
    if (var_arr[i] == NULL) {
        return 0;
    } else {
        struct si_node * t = var_arr[i];
        while (strncmp(t->s, var_name, strlen(var_name)) != 0) {
            if (t->next == NULL) return 0;
            else t = t->next;
        }
    }
    return 1;
}
void delete_table(struct si_node** var_arr, int n) {
    for (int i = 0; i < n; i++) {
        if (var_arr[i] == NULL) continue;
        struct si_node * t = var_arr[i];
        struct si_node * del;
        do {
            del = t;
            t = t->next;
            free(del->s);
            free(del);
        } while (t != NULL);
        var_arr[i] = NULL;
    }
}

// Expr Tree

struct exp_node * new_node_from_num(int num) {
    struct exp_node * t = (struct exp_node *) malloc(sizeof(struct exp_node));
    t->type = NUMBER;
    t->num = num;
    t->l = NULL; t->r = NULL;
    return t;
}
struct exp_node * new_node_from_id(char* id) {
    struct exp_node * t = (struct exp_node *) malloc(sizeof(struct exp_node));
    t->type = IDENTIFIER;
    int l = strlen(id)+1;
    t->id = (char*) malloc(l);
    strncpy(t->id, id, l);
    t->l = NULL; t->r = NULL;
    return t;
}
struct exp_node * new_node_from_op(char op, struct exp_node * l, struct exp_node * r) {
    struct exp_node * t = (struct exp_node *) malloc(sizeof(struct exp_node));
    t->type = OPERATOR;
    t->op = op;
    t->l = l;
    t->r = r;
    return t;
}
void delete_exp(struct exp_node * t) {
    if (t == NULL) return;
    delete_exp(t->l);
    delete_exp(t->r);
    if (t->type == IDENTIFIER) {
        free(t->id);
    }
    free(t);
}
int eval(struct exp_node* exp) {
    if (exp->type == NUMBER) return exp->num;
    if (exp->type == IDENTIFIER) {
        int num;
        get_var(var_table, TABLE_SIZE, exp->id, &num);
        return num;
    }
    int a, b, c;
    a = eval(exp->l);
    b = eval(exp->r);
    switch (exp->op) {
        case '+':
            c = a + b;
            break;
        case '-':
            c = a - b;
            break;
        case '*':
            c = a * b;
            break;
        case '/':
            c = a / b;
            break;
        case '^':
            for (int i = 0, c = 1; i < b; i++) {
                c *= a;
            }
            break;
        case '=':
            c = a == b;
            break;
        case '<':
            c = a < b;
            break;
        case '>':
            c = a > b;
            break;
        default:
            fprintf(stderr, "Error: Unknown operator.\n");
    }
    return c;
}


// Command List

struct cmd_node * new_if(struct exp_node * exp, struct cmd_node * cmd_list1, struct cmd_node * cmd_list2) {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = IF;
    t->_if_.exp = exp;
    t->_if_.cmd_list1 = cmd_list1;
    t->_if_.cmd_list2 = cmd_list2;
    t->next = NULL;
    return t;
}
struct cmd_node * new_while(struct exp_node * exp, struct cmd_node * cmd_list) {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = WHILE;
    t->_while_.exp = exp;
    t->_while_.cmd_list = cmd_list;
    t->next = NULL;
    return t;
}
struct cmd_node * new_assign(char* identifier, struct exp_node* exp) {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = ASSIGN;
    int l = strlen(identifier)+1;
    t->_assign_.identifier = (char*) malloc(l);
    strncpy(t->_assign_.identifier, identifier, l);
    t->_assign_.exp = exp;
    t->next = NULL;
    return t;
}
struct cmd_node * new_read(char* identifier) {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = READ;
    int l = strlen(identifier)+1;
    t->_read_.identifier = (char*) malloc(l);
    strncpy(t->_read_.identifier, identifier, l);
    t->next = NULL;
    return t;
}
struct cmd_node * new_write(struct exp_node* exp) {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = WRITE;
    t->_write_.exp = exp;
    t->next = NULL;
    return t;
}
struct cmd_node * new_skip() {
    struct cmd_node * t = (struct cmd_node *) malloc(sizeof(struct cmd_node));
    t->type = SKIP;
    t->next = NULL;
    return t;
}
void delete_cmd_list(struct cmd_node * t) {
    if (t == NULL) return;
    delete_cmd_list(t->next);
    switch (t->type) {
        case IF:
            delete_exp(t->_if_.exp);
            delete_cmd_list(t->_if_.cmd_list1);
            delete_cmd_list(t->_if_.cmd_list2);
            break;
        case WHILE:
            delete_exp(t->_while_.exp);
            delete_cmd_list(t->_while_.cmd_list);
            break;
        case ASSIGN:
            free(t->_assign_.identifier);
            delete_exp(t->_assign_.exp);
            break;
        case READ:
            free(t->_read_.identifier);
            break;
        case WRITE:
            delete_exp(t->_write_.exp);
            break;
        case SKIP:
            break;
    }
    free(t);
}
void exec(struct cmd_node * t) {
    if (t == NULL) return;
    switch (t->type) {
        case IF:
            if (eval(t->_if_.exp)) exec(t->_if_.cmd_list1);
            else exec(t->_if_.cmd_list2);
            break;
        case WHILE:
            while (eval(t->_while_.exp))
                exec(t->_while_.cmd_list);
            break;
        case ASSIGN:
            update_var(var_table, TABLE_SIZE, t->_assign_.identifier, eval(t->_assign_.exp));
            break;
        case READ:
            {
                int num;
                scanf("%d", &num);
                update_var(var_table, TABLE_SIZE, t->_read_.identifier, num);
            }
            break;
        case WRITE:
            printf("%d\n", eval(t->_write_.exp));
            break;
        case SKIP:
            break;
        default:
            fprintf(stderr, "Error: Unknown command\n");
    }
    exec(t->next);
}
