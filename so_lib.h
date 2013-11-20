#ifndef SO_LIB_H
#define SO_LIB_H

extern const char kill_message[42];
extern const size_t  PIPE_SIZE;

/**
* Returns 1 if c is an operator or else 0
* Operators are: + - / *
*/
int isOperator(char c);

/**
* Checks if given string is an Expression or a result.
* Returns position of first operator should it be an 
* expression or 0 otherwise.
*/
int isExpression(char* str);

/**
* Given an expression as string returns the position
* of the end of the second number in the formula to be counted.
* For expression "1 23 567 - +" it would return the position of 7.
*/
int getSecondEndPosition(char* str, int intOperator);

/**
* Given an expression as string returns the position
* of the end of the first number in the formula to be counted.
* For expression "1 23 567 - +" it would return the position of 3.
*/
int getFirstEndPosition(char* str, int intOperator);

/**
* Given an expression as string returns the position
* of the beginning of the first number in the formula to be counted.
* For expression "1 23 567 - +" it would return the position of 2.
*/
int getFirstStartPosition(char* str, int intOperator);

/**
* Given an expression as string and a position in it 
* returns the number ending at the position. If the 
* position indicates an operator or ' ' then it returns
* the longest integer ending no further than position.
*/
int getNumber(char* str, int position);

/**
* Given two integers and an operator returns the value of the operation.
*/
int compute(int n1, int n2, char c);

#endif