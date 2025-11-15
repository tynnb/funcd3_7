#ifndef FUNCS
#define FUNCS

#include <stdio.h>

#define MAX_VARIABLE_COUNT 26
#define STRING_LENGTH 256
#define LOG_FILE "trace.log"

typedef struct {
    int value;
    int initialized;
} Variable;

extern Variable variables[MAX_VARIABLE_COUNT];
extern int string_number;
extern FILE *log_file;

void initialization_variables(void);
int get_index(char name);
void log_operations(const char* command, const char* operation);
int parse_expression(const char** expr);
void process_command(const char* command);
void process_assignment(const char* command);
void process_print(const char* command);

#endif