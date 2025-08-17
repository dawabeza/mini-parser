#include "eval.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

extern  struct var_name_val  var_name_map[MAX_VAR_COUNT];
extern int var_namep;
// Evaluate the expression represented by the linked list
long double evaluate_expr(struct op_node **head, struct op_node **tail) {
    struct op_node *cur = *head;

    // Insert implicit multiplications (like 2(3) -> 2*3)
    standardize_expr(head, tail, cur);

    // Evaluate all nested expressions recursively
    simplify_nested(head, tail, cur);

    // Handle unary operator and right associative operators
    cur = *tail;
    simplify_func(head, tail, cur);
    simplify_unary(head, tail, cur, OPERATOR_PLUS | OPERATOR_MINUS);

    simplify_binary_right_assoc(head, tail, cur, OPERATOR_EXPONENT);
    // Handle binary operators in order of precedence

    cur = *head;
    simplify_binary_left_assoc(head, tail, cur, OPERATOR_DIVISION | OPERATOR_MULTIPLICAITON | OPERATOR_MODULO);
    simplify_binary_left_assoc(head, tail, cur, OPERATOR_PLUS | OPERATOR_MINUS);
    simplify_binary_left_assoc(head, tail, cur, OPERATOR_EQUAL);
    return (*head) ? (*head)->value : 0;
}

void simplify_func(struct op_node **head, struct op_node **tail, struct op_node *cur)
{
    while (cur) {
        if (cur->op_type == FUNC) {
            struct op_node *temp = cur;
            if (is_operator(cur->next) || cur->next->op_type != NESTED_EXPR) {
                printf("INVALID FUNC: %s", cur->name);
                exit(1);
            }
            cur->next->value = func_call(cur->name, cur->next->value);
            cur = cur->prev;
            remove_node(head, tail, temp);
        }
        else {
            cur = cur->prev;
        }

    }
}

// Insert multiplication nodes where two operands are adjacent
void standardize_expr(struct op_node **head, struct op_node **tail, struct op_node *cur) {
    while (cur && cur->next) {
        if ((cur->op_type == NUMBER || cur->op_type == NESTED_EXPR || cur->op_type == VAR) && 
            (cur->next->op_type == NUMBER || cur->next->op_type == NESTED_EXPR || cur->next->op_type == VAR)) {
            struct op_node *new_node = malloc(sizeof(struct op_node));
            init_node(new_node, INVALID_VALUE, OPERATOR_MULTIPLICAITON);
            insert_node(head, tail, &cur, new_node);
        }
        cur = cur->next;
    }
}

// Simplify unary operators applied to numbers
void simplify_unary(struct op_node **head, struct op_node **tail, struct op_node *cur, int type) {

    while (cur) {
        struct op_node *temp = cur;
        if (is_operator(cur) && (cur->op_type & type) && is_operator(cur->prev)) {
            struct op_node *temp = cur;
            if (cur->op_type == OPERATOR_MINUS) {
                cur->next->value *= -1;
            }
            cur = cur->prev;
            remove_node(head, tail, temp);
        }
        else {
            cur = cur->prev;
        }
    }
}

// Simplify binary operators like +, -, *, /, %
void simplify_binary_left_assoc(struct op_node **head, struct op_node **tail, struct op_node *cur, int type) {
    while (cur) {
        if (is_operator(cur) && (cur->op_type & type)) {
            long double result;
           //this is arithmetic expression evaluators
            perform_operation(cur, &result);
            cur->op_type = NUMBER;
            cur->value = result;
            struct op_node *temp1 = cur->prev;
            struct op_node *temp2 = cur->next;
            // Re-link the list
            cur = cur->next->next;
            remove_node(head, tail, temp1);
            remove_node(head, tail, temp2);
        }
         else {
            cur = cur->next;
        }
    }
}

void simplify_binary_right_assoc(struct op_node **head, struct op_node **tail, struct op_node *cur, int type)
{
    while (cur) {
        if (is_operator(cur) && (cur->op_type & type)) {
            long double result;
           //this is arithmetic expression evaluators
            perform_operation(cur, &result);
            cur->op_type = NUMBER;
            cur->value = result;
            struct op_node *temp1 = cur->prev;
            struct op_node *temp2 = cur->next;
            // Re-link the list
            cur = cur->next->next;
            remove_node(head, tail, temp1);
            remove_node(head, tail, temp2);
        }
         else {
            cur = cur->prev;
        }
    }

}

// Evaluate all nested expressions recursively
void simplify_nested(struct op_node **head, struct op_node **tail, struct op_node *cur) {
    while (cur) {
        if (cur->op_type == NESTED_EXPR) {
            cur->value = evaluate_expr(&cur->expr_head, &cur->expr_tail);
        }
        cur = cur->next;
    }
}
