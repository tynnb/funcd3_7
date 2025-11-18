#include "funcs.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        return 1;
    }

    initialization_variables();

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        process_command(line);
    }

    fclose(input_file);
    if (log_file) {
        fclose(log_file);
    }
    
    return 0;
}