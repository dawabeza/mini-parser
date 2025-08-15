#ifndef EVAL_H
#define EVAL_H

#include "op_node.h"


long double evaluate_expr(struct op_node **head, struct op_node **tail);

void simplify_func(struct op_node **head, struct op_node **tail, struct op_node *cur);
void simplify_unary(struct op_node **head, struct op_node **tail, struct op_node *cur, int type);
void simplify_binary(struct op_node **head, struct op_node **tail, struct op_node *cur, int type);
void simplify_nested(struct op_node **head, struct op_node **tail, struct op_node *cur);
void standardize_expr(struct op_node **head, struct op_node **tail, struct op_node *cur);

void perform_operation(struct op_node *cur, long double *result);
#endif
