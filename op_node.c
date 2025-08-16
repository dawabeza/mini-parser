#include "op_node.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize a new op_node with the given value and type
void init_node(struct op_node *node, long double value, enum op_type op_type) {
    node->prev = NULL;
    node->next = NULL;
    node->expr_head = NULL;
    node->expr_tail = NULL;
    node->value = value;
    node->op_type = op_type;
}

// Append a new node to the end of the doubly linked list
void list_append(struct op_node **head, struct op_node **tail, struct op_node *new_node) {
    if (!(*head)) {
        *head = *tail = new_node;
    } else {
        new_node->prev = *tail;
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

void remove_node(struct op_node **head, struct op_node **tail, struct op_node *node)
{
    if (!node) return;
    if (!node->prev && !node->next) {
        *head = *tail = NULL;
    }
    else if (!node->prev) {
        *head = node->next;
        (*head)->prev = NULL;
        node->next = NULL;
        node->next = NULL;
    }
    else if (!node->next) {
        *tail = node->prev;
        (*tail)->next = NULL;
        node->prev = NULL;
        node->next = NULL;
    }
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
    }
    free(node);
    node = NULL;
}

// Free the linked list and any nested expressions recursively
void free_list(struct op_node *head, struct op_node *tail) {
    while (head != NULL) {
        struct op_node *next = head->next;

        if (head->op_type == NESTED_EXPR) {
            free_list(head->expr_head, head->expr_tail);
        }

        free(head);
        head = next;
    }
}

// Print the linked list for debugging purposes
void print_list(struct op_node *head) {
    while (head) {
        if (head->op_type == NUMBER) {
            printf(" %Lf ", head->value);
        } 
        else if (head->op_type == NESTED_EXPR) {
            printf("(");
            print_list(head->expr_head);  // Recursively print nested expressions
            printf(")");
        } 
        
        else {
            switch (head->op_type) {
                case OPERATOR_DIVISION:       printf(" / "); break;
                case OPERATOR_MULTIPLICAITON: printf(" * "); break;
                case OPERATOR_PLUS:           printf(" + "); break;
                case OPERATOR_MINUS:          printf(" - "); break;
                case OPERATOR_MODULO:         printf(" %% "); break;
                case OPERATOR_EQUAL:          printf(" = "); break;
                case OPERATOR_EXPONENT:       printf(" ^ "); break;
                default: break;
            }
        }
        head = head->next;
    }
    printf("\n");
}

void insert_node(struct op_node **head, struct op_node **tail, struct op_node **insert_to, struct op_node *new_node)
{
     if (!insert_to) {
        head = tail = insert_to;
        return;
     }
     new_node->next = (*insert_to)->next;
     new_node->prev = *insert_to;
     (*insert_to)->next->prev = new_node;
     (*insert_to)->next = new_node;
}

// Convert a character into the corresponding operator type
enum op_type get_op_type(char c) {
    switch(c) {
        case '/': return OPERATOR_DIVISION;
        case '*': return OPERATOR_MULTIPLICAITON;
        case '+': return OPERATOR_PLUS;
        case '-': return OPERATOR_MINUS;
        case '%': return OPERATOR_MODULO;
        case '=': return OPERATOR_EQUAL;
        case '^': return OPERATOR_EXPONENT;
        default:  return NONE;
    }
}

// Check if a node represents an operator
bool is_operator(struct op_node *node) {
    if (!node) return true;  // Treat NULL as operator for error handling
    return       node->op_type == OPERATOR_MULTIPLICAITON || node->op_type == OPERATOR_DIVISION ||
                 node->op_type ==  OPERATOR_EQUAL || node->op_type ==  OPERATOR_PLUS  || 
                 node->op_type == OPERATOR_MINUS || node->op_type == OPERATOR_EXPONENT;
}

