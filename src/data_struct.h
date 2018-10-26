#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct si_node {
    char* s;
    int i;
    struct si_node* next;
};
struct exp_node {
    int type;
    union {
        char op;
        char* id;
        int num;
    };
    struct exp_node *l, *r;
};
struct cmd_node {
    int type;
    union {
        struct {
            struct exp_node * exp;
            struct cmd_node * cmd_list1, * cmd_list2;
        } _if_;
        struct {
            struct exp_node * exp;
            struct cmd_node * cmd_list;
        } _while_;
        struct {
            char* identifier;
            struct exp_node* exp;
        } _assign_;
        struct {
            char* identifier;
        } _read_;
        struct {
            struct exp_node* exp;
        } _write_;
    };
    struct cmd_node* next;
};

#include "main.h"

// Hash table

int num(char c);
int hash(const char * s, int n);
void __new_var(struct si_node* t, char* var_name, int var_num);
int add_var(struct si_node** var_arr, int n, char* var_name, int var_num);
int update_var(struct si_node** var_arr, int n, char* var_name, int var_num);
int get_var(struct si_node** var_arr, int n, char* var_name, int* var_num);
int exist_var(struct si_node** var_arr, int n, char* var_name);
void delete_table(struct si_node** var_arr, int n);


// Expr Tree

#define OPERATOR 10
#define IDENTIFIER 11
#define NUMBER 12

struct exp_node * new_node_from_num(int num);
struct exp_node * new_node_from_id(char* id);
struct exp_node * new_node_from_op(char op, struct exp_node * l, struct exp_node * r);
void delete_exp(struct exp_node * t);
int eval(struct exp_node* exp);


// Command List

#define IF 1
#define WHILE 2
#define ASSIGN 3
#define READ 4
#define WRITE 5
#define SKIP 6

struct cmd_node * new_if(struct exp_node * exp, struct cmd_node * cmd_list1, struct cmd_node * cmd_list2);
struct cmd_node * new_while(struct exp_node * exp, struct cmd_node * cmd_list);
struct cmd_node * new_assign(char* identifier, struct exp_node* exp);
struct cmd_node * new_read(char* identifier);
struct cmd_node * new_write(struct exp_node* exp);
struct cmd_node * new_skip(void);
void delete_cmd_list(struct cmd_node * t);
void exec(struct cmd_node * t);


#endif