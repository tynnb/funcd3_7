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

StatusCode log_operations(const char* command, const char* operation) {
    if (log_file == NULL) {
        log_file = fopen(LOG_FILE, "w");
        if (log_file == NULL) {
            return ERROR_FILE_OPEN;
        }
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
    return SUCCESS;
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
    int result = parse_power(expr);

    while (1) {
        while (**expr == ' ') {
            (*expr)++;
        }

        char op = **expr;
        if (op != '*' && op != '/' && op != '^') {
            break;
        }
        (*expr)++;

        int right = parse_power(expr);

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

StatusCode process_command(const char* command) {
    string_number++;

    while (*command == ' ') {
        command++;
    }

    if (strncmp(command, "print", 5) == 0) {
        return process_print(command);
    }
    else if (*command >= 'A' && *command <= 'Z') {
        const char* temp = command + 1;
        while (*temp == ' ') {
            temp++;
        }
        if (*temp == '=') {
            return process_assignment(command);
        }
        else {
            return ERROR_INVALID_COMMAND;
        }
    }
    else {
        return ERROR_INVALID_COMMAND;
    }
}

StatusCode process_assignment(const char* command) {
    char var_name = *command;
    int var_index = get_index(var_name);

    const char* expr = command + 1;
    while (*expr == ' ') expr++;
    
    if (*expr != '=') {
        return ERROR_INVALID_COMMAND;
    }
    expr++;
    
    while (*expr == ' ') expr++;
    
    int value = parse_expression(&expr);

    variables[var_index].value = value;
    variables[var_index].initialized = 1;

    const char* operation_type = "Assignment";
    
    const char* after_equals = strchr(command, '=');
    if (after_equals) {
        after_equals++;
        int has_operations = 0;
        int has_non_space = 0;
        
        while (*after_equals != '\0') {
            if (*after_equals != ' ') {
                has_non_space++;
                if (*after_equals == '+' || *after_equals == '-' || 
                    *after_equals == '*' || *after_equals == '/' || 
                    *after_equals == '^') {
                    has_operations = 1;
                    break;
                }
            }
            after_equals++;
        }
        
        if (has_operations && has_non_space > 1) {
            operation_type = "Arithmetic operation";
        }
    }

    return log_operations(command, operation_type);
}

StatusCode process_print(const char* command) {
    const char* expr = command + 5;
    while (*expr == ' ') expr++;
    if (*expr != '(') {
        return ERROR_INVALID_COMMAND;
    }

    expr++;
    int value = parse_expression(&expr);

    while (*expr == ' ') expr++;
    if (*expr != ')') {
        return ERROR_INVALID_COMMAND;
    }

    printf("%d\n", value);
    return log_operations(command, "Print");
}

int parse_factor(const char** expr) {
    while (**expr == ' ') {
        (*expr)++;
    }
    
    if (**expr == '(') {
        (*expr)++;
        int result = parse_expression(expr);
        while (**expr == ' ') {
            (*expr)++;
        }
        if (**expr == ')') {
            (*expr)++;
        }
        return result;
    }
    
    return parse_value(expr);
}

int parse_power(const char** expr) {
    int result = parse_factor(expr);
    
    while (1) {
        while (**expr == ' ') (*expr)++;
        
        char op = **expr;
        if (op != '^') break;
        
        (*expr)++;
        int right = parse_factor(expr);
        result = (int)pow(result, right);
    }
    return result;
}