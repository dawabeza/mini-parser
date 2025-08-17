#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char *math_funcs[] = {"sin", "cos", "tan", "asin", "acos", "atan", "sinh", 
                          "cosh", "tanh", "exp", "ln", "log", "sqrt", "pow", "abs", "print"};


struct symb_const symb_map[] = { 
            {"e", 2.71828182845904523536L},
            {"pi", 3.14159265358979323846L},
            {"gamma", 0.57721566490153286060L},
            {"phi", 1.61803398874989484820L}
  };

struct statement_list *statement_head, *statement_tail;

int prev_statement = 0;

struct var_name_val  var_name_map[MAX_VAR_COUNT];
//pointer for  above map for tracking back position
int var_namep = 0;

//here get input with ignoring white space
int get_statement(char *expr) {
    int c;
    int i = 0;

    while (i < MAX_OPERAND && (c = getchar()) != EOF) {
        if (!isspace(c) && c != '\n') {
            if (c == ';') {
                expr[i] = '\0';
                return i;
            }
            expr[i++] = c;
        }
    }

    return -1;
}

// Converts the input expression into a linked list of operator nodes
void get_op_list(char *expr, struct op_node **head, struct op_node **tail, int start, int end) {
    int cur = start;

    while (cur <= end) {
        if (isdigit(expr[cur])) {
            cur = handle_digit(expr, head, tail, cur, end);
        } 
        else if (expr[cur] == '(') {
            cur = handle_nested_expr(expr, head, tail, cur, end);
        } 
        else if(isalpha(expr[cur])) {
            int temp;
            //here the order matter, first check var if not check the next
            if ((temp = handle_var(expr, head, tail, cur, end)) != -1) {
                cur = temp;
            }
            else if ((temp = handle_math_func(expr, head, tail, cur, end)) != -1) {
                cur = temp;
            }
            else if ((temp = handle_symb_const(expr, head, tail, cur, end)) != -1) {
                cur = temp;
            }
        }
        else if (get_op_type(expr[cur]) != NONE) {
            cur = handle_op(expr, head, tail, cur, end);
        } 
        else if (expr[cur] == ')') { 
            printf("Invalid closing parenthesis at: %d\n", cur); 
            exit(1);
        } 
        else {
            printf("Invalid character \'%c\' at: %d\n", expr[cur], cur);
            exit(1);
        }
    }
}


int handle_var(char *expr, struct op_node **head, struct op_node **tail, int start, int end)
{
    char var_name[MAX_VAR_SIZE];
    int i = 0;
    while (isalpha(expr[start]) || expr[start] == '_') {
        var_name[i++] = expr[start];
        start++;
    }
    var_name[i] = '\0';
    if (is_func(var_name) || is_symb_const(var_name)) {
        return -1;
    }

    struct op_node *new_node = malloc(sizeof(struct op_node));
    long double val = INVALID_VALUE;
    int index;
    if ((index = find_var(var_name)) != -1) {
        val = var_name_map[index].val;
    }
    init_node(new_node, val, VAR);
    strcpy(new_node->name, var_name);
    list_append(head, tail, new_node);
    return start;
}

int handle_symb_const(char *expr, struct op_node **head, struct op_node **tail, int start, int end)
{
    char var_name[MAX_VAR_SIZE];
    int i = 0;
    while (isalpha(expr[start])) {
        var_name[i++] = expr[start];
        start++;
    }
    var_name[i] = '\0';
    if (is_symb_const(var_name)) {
        struct op_node *new_node = malloc(sizeof(struct op_node));
        long double value;
        for (int i = 0; i < SYMB_CONST_COUNT; i++) {
            if (strcmp(symb_map[i].symb_name, var_name) == 0) {
                value = symb_map[i].val;
                break;
            }
        }
        init_node(new_node, value, NUMBER);
        list_append(head, tail, new_node);
        return start;
    }

    return -1;
}

// Finds the matching closing parenthesis for an opening parenthesis
int closing_parenthesis(char *expr, int paren_pos, int end) {
    if (expr[paren_pos] != '(') return -1;

    int stackp = 1;
    paren_pos++;
    while (paren_pos <= end) {
        if (expr[paren_pos] == '(') {
            stackp++;
        } 
        else if (expr[paren_pos] == ')') {
            stackp--;
            if (stackp == 0) return paren_pos;
        }
        paren_pos++;
    }

    return -1; // no matching parenthesis found
}

int handle_math_func(char *expr, struct op_node **head, struct op_node **tail, int start, int end)
{
    char func_name[MAX_VAR_SIZE];
    int i = 0;
    while (isalpha(expr[start])) {
        func_name[i++] = expr[start];
        start++;
    }
    func_name[i] = '\0';
    if (is_func(func_name)) {
        struct op_node *new_node = malloc(sizeof(struct op_node));

        init_node(new_node, INVALID_VALUE, FUNC);
        strcpy(new_node->name, func_name);
        list_append(head, tail, new_node);
        return start;
    }

    return -1;
}

// Handles numbers (integer, floating-point, scientific notation)
int handle_digit(char *expr, struct op_node **head, struct op_node **tail, int start, int end) {
    long double num = 0;

    // Parse integer part
    while (isdigit(expr[start])) {
        num = num * 10 + expr[start++] - '0';
    }

    // Parse fractional part
    long double frac = 0;
    int power = 1;
    if (expr[start] == '.') {
        start++;
        while (isdigit(expr[start])) {
            frac = frac * 10 + expr[start++] - '0';
            power *= 10;
        }
    }

    // Parse scientific notation
    int exp = 0;
    int sign = 1;
    if (expr[start] == 'e' || expr[start] == 'E') {
        start++;
        if (expr[start] == '-') {
            sign = -1;
            start++;
        } else if (expr[start] == '+') {
            start++;
        }

        while (isdigit(expr[start])) {
            exp = exp * 10 + expr[start++] - '0';
        }
    }

    num += frac / power;
    exp *= sign;
    num *= pow(10, exp);


    // Create new node and append to list
    struct op_node *new_node = malloc(sizeof(struct op_node));
    init_node(new_node, num, NUMBER);
    list_append(head, tail, new_node);

    return start;
}

// Handles nested expressions inside parentheses
int handle_nested_expr(char *expr, struct op_node **head, struct op_node **tail, int start, int end) {
    int closing_paren = closing_parenthesis(expr, start, end);
    if (closing_paren == -1) {
        printf("ERROR: No closin parenthesis: at %d\n", start);
        exit(1);
    }

    struct op_node *new_node = malloc(sizeof(struct op_node));
    init_node(new_node, INVALID_VALUE, NESTED_EXPR);

    // Recursively parse the nested expression
    get_op_list(expr, &new_node->expr_head, &new_node->expr_tail, start + 1, closing_paren - 1);

    list_append(head, tail, new_node);

    return closing_paren + 1;
}

// Handles operators like +, -, *, /, %
int handle_op(char *expr, struct op_node **head, struct op_node **tail, int start, int end) {
    if (start >= end) {
        printf("ERROR: trailing Operator: at %d\n", start);
        exit(1);
    }

    struct op_node *new_node = malloc(sizeof(struct op_node));
    init_node(new_node, -1, get_op_type(expr[start]));
    list_append(head, tail, new_node);

    return start + 1;
}

bool is_func(char *name)
{ 
    for (int i = 0; i < MAX_FUNC_COUNT; i++) {
        if (strcmp(name, math_funcs[i]) == 0) {
            return true;
        }
    }

    return false;
}

bool is_symb_const(char *name)
{
    for (int i = 0; i < SYMB_CONST_COUNT; i++) {
        if (strcmp(name,  symb_map[i].symb_name) == 0) {
            return true;
        }
    }
    return false;
}

int find_var(char *var_name)
{
    for (int i = 0; i < var_namep; i++) {
        if (strcmp(var_name, var_name_map[i].var_name) == 0) {
            return i;
        }
    }
    return -1;
}

void print_var()
{
    printf("\n");
    
    for (int i = 0; i < var_namep; i++) {
        if (var_name_map[i].var_name[0] != '\0') {
            printf("%s: %Lf  ", var_name_map[i].var_name, var_name_map[i].val);
        }
    }
    //for flushing buffer
    printf("\n");
}

long double func_call(char *func_name, long double arg1)
{
    if (strcmp(func_name, "sin") == 0) return sin(arg1);
    if (strcmp(func_name, "cos") == 0) return cos(arg1);
    if (strcmp(func_name, "tan") == 0) return tan(arg1);
    if (strcmp(func_name, "asin") == 0) return asin(arg1);
    if (strcmp(func_name, "acos") == 0) return acos(arg1);
    if (strcmp(func_name, "atan") == 0) return atan(arg1);
    if (strcmp(func_name, "sinh") == 0) return sinh(arg1);
    if (strcmp(func_name, "cosh") == 0) return cosh(arg1);
    if (strcmp(func_name, "tanh") == 0) return tanh(arg1);
    if (strcmp(func_name, "exp") == 0) return exp(arg1);
    if (strcmp(func_name, "abs") == 0) return abs(arg1);
    if (strcmp(func_name, "ln") == 0) return log(arg1);
    if (strcmp(func_name, "log") == 0) return log10(arg1);
    if (strcmp(func_name, "sqrt") == 0) return sqrt(arg1);

    if (strcmp(func_name, "print") == 0) 
        printf("%Lf\n", arg1);
    return 0;
}

void set_var(char *var_name, long double val)
{
    int index;
    if ((index = find_var(var_name)) != -1) {
        var_name_map[index].val = val;
    } else {
        strcpy(var_name_map[var_namep].var_name, var_name);
        var_name_map[var_namep].val = val;
        var_namep++;
    }
}

void perform_operation(struct op_node *cur, long double *result)
{
    if (is_operator(cur->next) || is_operator(cur->prev)) {
        printf("ERROR: INVALID OPERATOR\n");
        exit(1);
    }
     switch(cur->op_type) {
        case OPERATOR_DIVISION: 
            if (cur->next->op_type )
            *result = cur->prev->value / cur->next->value; 
            break;
        case OPERATOR_MULTIPLICAITON: 
            *result = cur->prev->value * cur->next->value; 
            break;
        case OPERATOR_PLUS: 
            *result = cur->prev->value + cur->next->value; 
            break;
        case OPERATOR_MINUS: 
            *result = cur->prev->value - cur->next->value; 
        case OPERATOR_EXPONENT:
            *result = pow(cur->prev->value, cur->next->value);
            break;
        case OPERATOR_EQUAL: 
            if (cur->prev->op_type != VAR) {
                printf("ERROR: CAN'T ASSIGN TO NON_VARIABLE DATA\n");
                exit(1);
            }
            *result = cur->prev->value = cur->next->value;
            set_var(cur->prev->name, *result);
            break;
        case OPERATOR_MODULO: 
            *result = (int)cur->prev->value % (int)cur->next->value; 
            break;
        default: 
            *result = 0; 
            break;
    }

}
