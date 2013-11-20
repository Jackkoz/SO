#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "so_lib.h"

int main() {

    char ring_buffer[PIPE_SIZE];

    while (true) {
        //Clearing buffer before reading into it to avoid leftovers
        memset(&ring_buffer[0], 0, sizeof(ring_buffer));
        read(0, ring_buffer, PIPE_SIZE);
        if (strcmp(ring_buffer, kill_message) == 0) {
            //If kill_message is read the proceed to signal next process and exit
            write(1, kill_message, strlen(kill_message) + 1);
            return 0;
            exit(0);
        }

        if (isExpression(ring_buffer) == 0) {
            //The expression is already counted, pass it forward
            write(1, ring_buffer, strlen(ring_buffer) + 1);
        }
        else {
            //There is something to be counted, proceed
            int int1, int2, intOperator, result;
            intOperator = isExpression(ring_buffer); //position of the operator for current formula
            //Explained semantics of functions below can be found in so_lib.h
            int1 = getNumber(ring_buffer, getFirstEndPosition(ring_buffer, intOperator));
            int2 = getNumber(ring_buffer, getSecondEndPosition(ring_buffer, intOperator));
            result = compute(int1, int2, ring_buffer[intOperator]);
            //Clearing variables used to store prefix and sufix of the formula in current expression
            char prefix[strlen(ring_buffer)], sufix[strlen(ring_buffer)];
            int i = 0;
            for (i; i < strlen(ring_buffer); i++) {
                prefix[i] = '\0';
                sufix[i] = '\0';
            }
            //The expression is reconstructed using the prefix,
            //result instead of the formula and then the sufix
            strncpy(prefix, ring_buffer, getFirstStartPosition(ring_buffer, intOperator));
            prefix[getFirstStartPosition(ring_buffer, intOperator)] = '\0';

            strncpy(sufix, ring_buffer + (intOperator + 1), strlen(ring_buffer) - intOperator);

            sprintf(ring_buffer, "%s%d%s", prefix, result, sufix);
            write(1, ring_buffer, strlen(ring_buffer) + 1);
        }
    }
}