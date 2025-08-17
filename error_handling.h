#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
struct error {
    int line;
    char *error_message;
};

void report_error();
#endif