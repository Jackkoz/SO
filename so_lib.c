// #include <unistd.h> //linux
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <errno.h>
#include "so_lib.h"


int isOperator(char c) {
	if (c == '*' || c == '+' || c == '-' || c == '/')
		return 1;
	else
		return 0;
}

int isExpression(char* str) {
	int position = 0;
	int number_count = 0;

	while (position < strlen(str)) {
		if (str[position] == ' ') {
			if (isOperator(str[position - 1]) != 0)
				number_count++;
		} else if (isOperator(str[position]) == 1) {
			if (number_count >= 2) 
				return position;
			else
				number_count = 0;
		} //else do nothing

		position++;
	}

	return 0;
}

int getFirstNumber(char* str, int position) {

	int iter = position - 2;
	while(str[iter] != ' ')
		iter--;

	char buffer[position - iter - 3];
	strncpy(buffer, str+iter+1, position - 2);

	return atoi(buffer);
}

int getSecondNumber(char* str, int position) {

	int iter = position - 2;
	while(str[iter] != ' ')
		iter--;
	iter--;
	int end = iter;
	while(str[iter] != ' ')
		iter --;

	char buffer[end - iter - 1];
	strncpy(buffer, str+iter+1, end);

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

int main() {return 0;}