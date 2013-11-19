#ifndef SO_LIB_H
#define SO_LIB_H

const char kill_message[4] = "###";

int isOperator(char c);

int isExpression(char* str);

int getFirstNumber(char* str, int position);

int getSecondNumber(char* str, int position);

int getNumber(char* str, int position);

#endif
