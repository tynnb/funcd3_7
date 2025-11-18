#include "funcs.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return ERROR_INVALID_COMMAND;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        return ERROR_FILE_OPEN;
    }

    initialization_variables();

    char line[STRING_LENGTH];
    
    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        StatusCode status = process_command(line);
        if (status != SUCCESS) {
            fclose(input_file);
            if (log_file) {
                fclose(log_file);
            }
            return status;
        }
    }

    fclose(input_file);
    if (log_file) {
        fclose(log_file);
    }
    
    return SUCCESS;
}