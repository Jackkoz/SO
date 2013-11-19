#ifndef SO_LIB_H
#define SO_LIB_H

const char kill_message[4] = "###";
const size_t  PIPE_SIZE = 64000;

int isOperator(char c);

int isExpression(char* str);

int getSecondEndPosition(char* str, int intOperator);

int getFirstEndPosition(char* str, int intOperator);

int getFirstStartPosition(char* str, int intOperator);

int getNumber(char* str, int position);

int compute(int n1, int n2, char c);

#endif
