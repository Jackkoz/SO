#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "so_lib.h"

const char kill_message[42] = "(>'-')> <('-'<) ^(' - ')^ <('-'<) (>'-')>";
const size_t  PIPE_SIZE = 65536;

int isOperator(char c) {
    if (c == '*' || c == '+' || c == '-' || c == '/')
        return 1;
    else
        return 0;
}

int isExpression(char* str) {
    int position = 0;

    while (position < strlen(str) - 1) {
        if (isOperator(str[position]) == 1 && 
                    (position == strlen(str) - 2 || str[position + 1] == ' '))
            return position;
        position++;
    }

    return 0;
}

int getSecondEndPosition(char* str, int intOperator) {
    return intOperator - 2;
}

int getFirstEndPosition(char* str, int intOperator) {
    int iter = intOperator - 2;
    while (str[iter] != ' ')
        iter--;
    return iter - 1;
}

int getFirstStartPosition(char* str, int intOperator) {
    int iter = intOperator - 2;
    while (str[iter] != ' ')
        iter--;

    iter--;
    while (str[iter] != ' ' && iter > 0)
        iter--;

    if (str[iter] == ' ')
        iter++;

    return iter;
}

int getNumber(char* str, int position) {    
    int iter = position;

    if (isOperator(str[iter]) == 1)
        iter--;
    if (str[iter] == ' ')
        iter--;

    while(str[iter] != ' ' && iter > 0)
        iter--;
    
    if (str[iter] == ' ')
        iter++;

    char buffer[position - iter + 2];
    strncpy(buffer, str+iter, position - iter + 1);
    buffer[position - iter + 1] = '\0';

    return atoi(buffer);    
}

int compute(int n1, int n2, char c) {
    switch (c) {
        case '+':
            return n1 + n2;
        case '-':
            return n1 - n2;
        case '*':
            return n1 * n2;
        case '/':
            return n1 / n2;
        default:
            //impossible
            return -1;
    }
}

void write_string(const char* str) {
    char temp_buffer[PIPE_SIZE], temp[PIPE_SIZE];

    sprintf(temp_buffer, "%d", strlen(str));
        while (strlen(temp_buffer) < 5) {
            sprintf(temp, "0%s", temp_buffer);
            sprintf(temp_buffer, "%s", temp);
        }
    write(1, temp_buffer, 6);
    write(1, str, strlen(str) + 1);
}