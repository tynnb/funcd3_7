#ifndef FUNCS
#define FUNCS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define MAX_VARIABLE_COUNT 26
#define STRING_LENGTH 256
#define LOG_FILE "trace.log"

typedef struct {
    int value;
    int initialized;
} Variable;

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_COMMAND,
    ERROR_VARIABLE_NOT_INITIALIZED,
    ERROR_DIVISION_BY_ZERO,
    ERROR_INVALID_EXPRESSION,
    ERROR_FILE_OPEN,
    ERROR_NULL_POINTER
} StatusCode;

extern Variable variables[MAX_VARIABLE_COUNT];
extern int string_number;
extern FILE *log_file;

void initialization_variables(void);
int get_index(char name);
StatusCode log_operations(const char* command, const char* operation);
int parse_expression(const char** expr);
StatusCode process_command(const char* command);
StatusCode process_assignment(const char* command);
StatusCode process_print(const char* command);
int parse_value(const char** expr);
int parse_term(const char** expr);

#endif