#ifndef PARSER_H
#define PARSER_H

#define MAX_FUNC_COUNT  16
#define MAX_VAR_COUNT  10000

#define SYMB_CONST_COUNT 4
#include "op_node.h"


struct symb_const {
    char symb_name[MAX_VAR_SIZE];
    long double val;
};

struct var_name_val {
    char var_name[MAX_VAR_SIZE];
    long double val;
};

struct statement_list {
    int statement_low;
    int statement_high;
};
int get_statement(char *expr);
void get_op_list(char *expr, struct op_node **head, struct op_node **tail, int start, int end);

int handle_var(char *expr, struct op_node **head, struct op_node **tail, int start, int end);
int handle_symb_const(char *expr, struct op_node **head, struct op_node **tail, int start, int end);
int handle_math_func(char *expr, struct op_node **head, struct op_node **tail, int start, int end);
int handle_digit(char *expr, struct op_node **head, struct op_node **tail, int start, int end);
int handle_nested_expr(char *expr, struct op_node **head, struct op_node **tail, int start, int end);
int handle_op(char *expr, struct op_node **head, struct op_node **tail, int start, int end);

bool is_func(char *name);
bool is_symb_const(char *name);
int find_var(char *var_name);
void print_var();
long double func_call(char *func_name,  long double arg1);
void set_var(char *var_name, long double val);
int closing_parenthesis(char* expr, int paren_pos, int end);
#endif
