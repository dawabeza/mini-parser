#include "execute.h"
#include "op_node.h"
#include "parser.h"
#include "eval.h"

void execute()
{
    int statement_start = 0;
    char statement[MAX_OPERAND];
    int statement_end;
    while ((statement_end = get_statement(statement)) != -1) {
        execute_statement(statement, statement_start, statement_end - 1);
        statement_start = statement_end +  1;
    }
}

void execute_statement(char *statement, int statement_start, int statement_end)
{
    struct op_node *head = NULL, *tail = NULL;
    get_op_list(statement, &head, &tail, statement_start, statement_end);
    evaluate_expr(&head, &tail);
}
