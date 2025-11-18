#include "funcs.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_initialization_variables() {
    printf("Testing initialization_variables...\n");
    
    initialization_variables();
    
    for (int i = 0; i < MAX_VARIABLE_COUNT; i++) {
        assert(variables[i].value == 0);
        assert(variables[i].initialized == 0);
    }
    
    printf("initialization_variables tests passed!\n");
}

void test_get_index() {
    printf("Testing get_index...\n");
    
    assert(get_index('A') == 0);
    assert(get_index('B') == 1);
    assert(get_index('Z') == 25);
    assert(get_index('M') == 12);
    
    assert(get_index('a') == -1);
    assert(get_index('1') == -1);
    assert(get_index('@') == -1);
    assert(get_index('[') == -1);
    
    printf("get_index tests passed!\n");
}

void test_parse_value() {
    printf("Testing parse_value...\n");
    
    const char* test1 = "42";
    const char* ptr1 = test1;
    int result1 = parse_value(&ptr1);
    assert(result1 == 42);
    
    const char* test2 = " 123 ";
    const char* ptr2 = test2;
    int result2 = parse_value(&ptr2);
    assert(result2 == 123);
    
    const char* test3 = "0";
    const char* ptr3 = test3;
    int result3 = parse_value(&ptr3);
    assert(result3 == 0);
    
    printf("parse_value tests passed!\n");
}

void test_parse_expression_simple() {
    printf("Testing parse_expression simple...\n");
    
    const char* test1 = "2 + 3";
    const char* ptr1 = test1;
    int result1 = parse_expression(&ptr1);
    assert(result1 == 5);
    
    const char* test2 = "10 - 4";
    const char* ptr2 = test2;
    int result2 = parse_expression(&ptr2);
    assert(result2 == 6);
    
    const char* test3 = "2 * 3";
    const char* ptr3 = test3;
    int result3 = parse_expression(&ptr3);
    assert(result3 == 6);
    
    const char* test4 = "8 / 2";
    const char* ptr4 = test4;
    int result4 = parse_expression(&ptr4);
    assert(result4 == 4);
    
    printf("parse_expression simple tests passed!\n");
}

void test_parse_expression_priority() {
    printf("Testing parse_expression priority...\n");
    
    const char* test1 = "2 + 3 * 4";
    const char* ptr1 = test1;
    int result1 = parse_expression(&ptr1);
    assert(result1 == 14);
    
    const char* test2 = "10 - 6 / 2";
    const char* ptr2 = test2;
    int result2 = parse_expression(&ptr2);
    assert(result2 == 7);
    
    const char* test3 = "2 * 3 + 4 * 5";
    const char* ptr3 = test3;
    int result3 = parse_expression(&ptr3);
    assert(result3 == 26);
    
    printf("parse_expression priority tests passed!\n");
}

void test_parse_expression_power() {
    printf("Testing parse_expression power...\n");
    
    const char* test1 = "2 ^ 3";
    const char* ptr1 = test1;
    int result1 = parse_expression(&ptr1);
    assert(result1 == 8);
    
    const char* test2 = "3 ^ 2";
    const char* ptr2 = test2;
    int result2 = parse_expression(&ptr2);
    assert(result2 == 9);
    
    const char* test3 = "2 + 3 ^ 2";
    const char* ptr3 = test3;
    int result3 = parse_expression(&ptr3);
    assert(result3 == 11);
    
    printf("parse_expression power tests passed!\n");
}

void test_process_assignment() {
    printf("Testing process_assignment...\n");
    
    initialization_variables();
    
    StatusCode status1 = process_assignment("A = 5");
    assert(status1 == SUCCESS);
    assert(variables[0].value == 5);
    assert(variables[0].initialized == 1);
    
    StatusCode status2 = process_assignment("B = A + 3");
    assert(status2 == SUCCESS);
    assert(variables[1].value == 8);
    assert(variables[1].initialized == 1);
    
    StatusCode status3 = process_assignment("C = 2 * B - A");
    assert(status3 == SUCCESS);
    assert(variables[2].value == 11);
    assert(variables[2].initialized == 1);
    
    printf("process_assignment tests passed!\n");
}

void test_process_print() {
    printf("Testing process_print...\n");
    
    initialization_variables();
    variables[0].value = 42;
    variables[0].initialized = 1;
    
    // Тестируем вывод - можно проверить через перехват stdout
    StatusCode status = process_print("print(A)");
    assert(status == SUCCESS);
    
    StatusCode status2 = process_print("print(2 + 3 * 4)");
    assert(status2 == SUCCESS);
    
    printf("process_print tests passed!\n");
}

void test_process_command() {
    printf("Testing process_command...\n");
    
    initialization_variables();
    
    StatusCode status1 = process_command("X = 10");
    assert(status1 == SUCCESS);
    assert(variables[23].value == 10);
    assert(variables[23].initialized == 1);
    
    // ОТЛАДКА: проверим значение X
    printf("DEBUG: X = %d, initialized = %d\n", variables[23].value, variables[23].initialized);
    
    StatusCode status2 = process_command("print(X)");
    assert(status2 == SUCCESS);
    
    StatusCode status3 = process_command("invalid command");
    assert(status3 == ERROR_INVALID_COMMAND);
    
    // ОТЛАДКА: перед проблемной строкой
    printf("DEBUG: Before Y = X * 2 + 1\n");
    StatusCode status4 = process_command("Y = X * 2 + 1");
    printf("DEBUG: status4 = %d\n", status4);  // какой статус возвращается?
    printf("DEBUG: Y value = %d, initialized = %d\n", variables[24].value, variables[24].initialized);
    assert(status4 == SUCCESS);
    assert(variables[24].value == 21);
    assert(variables[24].initialized == 1);
    
    printf("process_command tests passed!\n");
}

void test_variable_scope() {
    printf("Testing variable scope...\n");
    
    initialization_variables();
    
    process_command("A = 5");
    process_command("B = 10");
    process_command("C = A + B");
    
    assert(variables[0].value == 5);
    assert(variables[1].value == 10);
    assert(variables[2].value == 15);
    
    // Проверяем, что другие переменные не инициализированы
    assert(variables[3].initialized == 0);
    assert(variables[25].initialized == 0);
    
    printf("variable scope tests passed!\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");
    
    initialization_variables();
    
    // Деление на ноль
    //const char* expr1 = "5 / 0";
    //const char* ptr1 = expr1;
    //int result1 = parse_expression(&ptr1);
    // Проверяем что не упало и вернуло 0 или специальное значение
    
    // Неинициализированная переменная
    const char* expr2 = "Z";
    const char* ptr2 = expr2;
    int result2 = parse_value(&ptr2);
    assert(result2 == 0);
    
    // Неправильная команда
    StatusCode status1 = process_command("A 5"); // нет '='
    assert(status1 == ERROR_INVALID_COMMAND);
    
    StatusCode status2 = process_command("print A"); // нет скобок
    assert(status2 == ERROR_INVALID_COMMAND);
    
    printf("error handling tests passed!\n");
}

void test_complex_expressions() {
    printf("Testing complex expressions...\n");
    
    initialization_variables();
    process_command("A = 2");
    process_command("B = 3");
    process_command("C = 4");
    
    const char* expr1 = "A + B * C ^ 2";
    const char* ptr1 = expr1;
    int result1 = parse_expression(&ptr1);
    assert(result1 == 50); // 2 + 3 * 16 = 2 + 48 = 50
    
    const char* expr2 = "(A + B) * C";
    const char* ptr2 = expr2;
    int result2 = parse_expression(&ptr2);
    assert(result2 == 20); // (2 + 3) * 4 = 5 * 4 = 20
    
    printf("complex expressions tests passed!\n");
}

int main() {
    printf("Running interpreter tests...\n\n");
    
    test_initialization_variables();
    test_get_index();
    test_parse_value();
    test_parse_expression_simple();
    test_parse_expression_priority();
    test_parse_expression_power();
    test_process_assignment();
    test_process_print();
    test_process_command();
    test_variable_scope();
    test_error_handling();
    test_complex_expressions();
    
    printf("\nAll interpreter tests completed successfully!\n");
    return SUCCESS;
}