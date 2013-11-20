#include <unistd.h> //OS
#include <stdio.h> //sprintf
#include <stdlib.h> //exit
#include <string.h> //strlen
#include "so_lib.h"
#include <sys/wait.h>

int main() {

    char ring_buffer[PIPE_SIZE];

	read(0, ring_buffer, PIPE_SIZE);

    fprintf(stderr, "I'm #%d and got msg from #%s\n", getpid(), ring_buffer);

	sprintf(ring_buffer, "%d", getpid());
    write(1, ring_buffer, strlen(ring_buffer) + 1);
    
    while (true) {

        memset(&ring_buffer[0], 0, sizeof(ring_buffer));
        read(0, ring_buffer, PIPE_SIZE);
        if (strcmp(ring_buffer, kill_message) == 0) {
            fprintf(stderr, "KILLING MYSELF\n");
            write(1, kill_message, strlen(kill_message) + 1);

            return 0;
            exit(0);
        }

        if (isExpression(ring_buffer) == 0) {
            write(1, ring_buffer, strlen(ring_buffer) + 1);
        }
        else { //need to compute
            fprintf(stderr, "EXECUTOR COMPUTING: %s\n", ring_buffer);
            int int1, int2, intOperator, result;
            intOperator = isExpression(ring_buffer);
            int1 = getNumber(ring_buffer, getFirstEndPosition(ring_buffer, intOperator));
            int2 = getNumber(ring_buffer, getSecondEndPosition(ring_buffer, intOperator));
            result = compute(int1, int2, ring_buffer[intOperator]);

            //łączenie i wycinanie:
            char prefix[strlen(ring_buffer)], sufix[strlen(ring_buffer)]; 
            int i = 0;
            for (i; i < strlen(ring_buffer); i++) {
                prefix[i] = '\0';
                sufix[i] = '\0';
            }
            strncpy(prefix, ring_buffer, getFirstStartPosition(ring_buffer, intOperator));
            prefix[getFirstStartPosition(ring_buffer, intOperator)] = '\0';
            strncpy(sufix, ring_buffer + (intOperator + 1), strlen(ring_buffer) - intOperator);
            sprintf(ring_buffer, "%s%d%s", prefix, result, sufix);
            fprintf(stderr, "%s%d%s\n", prefix, result, sufix);
            fprintf(stderr, "Obliczone: %s\n", ring_buffer);
            write(1, ring_buffer, strlen(ring_buffer) + 1);
        }   
    }
}
