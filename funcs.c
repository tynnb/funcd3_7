#include "funcs.h"

Variable variables[MAX_VARIABLE_COUNT];
int string_number = 0;
FILE *log_file = NULL;

void initialization_variables(void) {
    for (int i = 0; i < MAX_VARIABLE_COUNT; i++) {
        variables[i].value = 0;
        variables[i].initialized = 0;
    }
}

int get_index(char name) {
    if (name >= 'A' && name <= 'Z') {
        return name - 'A';
    }
    else {
        return -1;
    }
}

void log_operations(const char* command, const char* operation) {
    if (log_file == NULL) {
        log_file = fopen(LOG_FILE, "a");
    }
    fprintf(log_file, "[%d] %s | ", string_number, command);

    int first = 1;
    for (int i = 0; i < MAX_VARIABLE_COUNT; i++) {
        if (variables[i].initialized) {
            if (!first) {
                fprintf(log_file, ", ");
            }
            fprintf(log_file, "%c=%d", 'A' + i, variables[i].value);
            first = 0;
        }
    }
    fprintf(log_file, " | %s\n", operation);
    fflush(log_file);
}

int parse_value(const char** expr) {
    while (**expr == ' ') {
        (*expr)++;
    }

    if (**expr >= 'A' && **expr <= 'Z') {
        int var_index = get_index(**expr);
        (*expr)++;
        if (variables[var_index].initialized) {
            return variables[var_index].value;
        }
        else {
            return 0;
        }
    }
    else if (**expr >= '0' && **expr <= '9') {
        int value = 0;
        while (isdigit(**expr)) {
            value = value * 10 + (**expr - '0');
            (*expr)++;
        }
        return value;
    }
    else {
        return 0;
    }
}

int parse_term(const char** expr) {
    int result = parse_value(expr);

    while (1) {
        while (**expr == ' ') {
            (*expr)++;
        }

        char op = **expr;
        if (op != '*' && op != '/' && op != '^') {
            break;
        }
        (*expr)++;

        int right = parse_value(expr);

        switch (op) {
            case '*':
                result *= right;
                break;
            case '/':
                if (right != 0) {
                    result /= right;
                }
                break;
            case '^':
                result = (int)pow(result, right);
                break;
        }
    }
    return result;
}

int parse_expression(const char** expr) {
    int result = parse_term(expr);

    while (1) {
        while (**expr == ' ') {
            (*expr)++;
        }

        char op = **expr;
        if (op != '+' && op != '-') {
            break;
        }
        (*expr)++;

        int right = parse_term(expr);

        switch (op) {
            case '+':
                result += right;
                break;
            case '-':
                result -= right;
                break;
        }
    }
    return result;
}

void process_command(const char* command) {
    string_number++;

    while (*command == ' ') {
        command++;
    }

    if (strncmp(command, "print", 5) == 0) {
        process_print(command);
    }
    else if (*command >= 'A' && *command <= 'Z') {
        const char* temp = command + 1;
        while (*temp == ' ') {
            temp++;
        }
        if (*temp == '=') {
            process_assignment(command);
        }
    }
}

void process_assignment(const char* command) {
    char var_name = *command;
    int var_index = get_index(var_name);

    const char* expr = command + 1;
    while (*expr == ' ') {
        expr++;
    }
    if (*expr != '=') {
        return;
    }
    expr++;
    while (*expr == ' ') {
        expr++;
    }
    int value = parse_expression(&expr);

    variables[var_index].value = value;
    variables[var_index].initialized = 1;

    log_operations(command, "Assignment");
}

void process_print(const char* command) {
    const char* expr = command + 5;
    while (*expr == ' ') expr++;
    if (*expr != '(') {
        return;
    }

    expr++;
    int value = parse_expression(&expr);

    while (*expr == ' ') expr++;
    if (*expr != ')') {
        return;
    }

    printf("%d\n", value);
    log_operations(command, "Print");
}