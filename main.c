#include "parser.h"
#include "eval.h"
#include <stdio.h>
#include <string.h>

int main() {
    struct op_node *head = NULL, *tail = NULL;
    char expr[MAX_OPERAND];
    int n;
    while ((n = get_expr(expr)) > 0) {
        get_op_list(expr, &head, &tail, 0, n - 1);
        evaluate_expr(&head, &tail);
        print_var();
        head = tail = NULL;
    }
    free_list(head, tail);
    return 0;
}
