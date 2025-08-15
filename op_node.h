#ifndef OP_NODE_H
#define OP_NODE_H

#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_OPERAND 1000
#define INVALID_VALUE 0
#define MAX_VAR_SIZE 100

enum op_type {
    NUMBER = (1 << 0), 
    NESTED_EXPR = (1 << 1), 
    OPERATOR_PLUS = (1 << 2),
    OPERATOR_MINUS = (1 << 3),
    OPERATOR_MULTIPLICAITON = (1 << 4),
    OPERATOR_DIVISION = (1 << 5),
    OPERATOR_MODULO = (1 << 6),
    OPERATOR_EQUAL = (1 << 7),
    FUNC,
    VAR,
    NONE,
};

struct op_node {
    struct op_node *expr_head;
    struct op_node *expr_tail;
    struct op_node *next;
    struct op_node *prev;
    char name[MAX_VAR_SIZE];
    long double value;
    enum op_type op_type;
};



// Linked list functions
void init_node(struct op_node *node, long double value, enum op_type op_type);
void list_append(struct op_node **head, struct op_node **tail, struct op_node *new_node);
void remove_node(struct op_node **head, struct op_node **tail, struct op_node *node);
void free_list(struct op_node *head, struct op_node *tail);
void print_list(struct op_node *head);
void insert_node(struct op_node **head, struct op_node **tail, struct op_node **insert_to, struct op_node *new_node);

// Helpers
enum op_type get_op_type(char c);
bool is_operator(struct op_node *node);

#endif
