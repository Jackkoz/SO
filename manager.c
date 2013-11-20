#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "so_lib.h"
#include <fcntl.h> 
#include <sys/wait.h>

int main(int arguments_number, char* arguments[]) {

    int process_count;         /* number of this process (starting with 1) */
    int childpid;              /* indicates process should spawn another   */
    int max_processes;         /* total number of processes in ring        */
    int new_descriptors[2];    /* file descriptors returned by pipe        */
    int error;                 /* return value from dup2 call              */
    int line_length;

    char file_buffer[PIPE_SIZE], temp_buffer[PIPE_SIZE], temp[PIPE_SIZE];

    if (arguments_number != 4) {
        fprintf (stderr, "Usage: %s max_processes, input_path, output_path\n", arguments[0]);
        exit(1);
    }

    max_processes = atoi(arguments[1]);

    /* check whether a valid number of processes to generate was specified*/
    if (max_processes <= 0) {
        fprintf (stderr, "Max processes should be greater than 0.\n", arguments[0]);
        exit(1);
    }

    if (pipe (new_descriptors) == -1) {
        perror("Could not create pipe");
        exit(1);
    }

    if ((dup2(new_descriptors[0], STDIN_FILENO) == -1) ||
            (dup2(new_descriptors[1], STDOUT_FILENO) == -1)) {
        perror("Could not dup pipes");
        exit(1);
    }

    if ((close(new_descriptors[0]) == -1) || (close(new_descriptors[1]) == -1)) {
        perror("Could not close extra descriptors");
        exit(1);
    }

    for (process_count = 1; process_count <= max_processes; process_count++) {
        if (pipe (new_descriptors) == -1) {
            fprintf(stderr,"Could not create pipe %d: %s\n",
                    process_count, strerror(errno));
            exit(1);
        }

        if ((childpid = fork()) == -1) {
            fprintf(stderr, "Could not create child %d: %s\n",
                   process_count, strerror(errno));
            exit(1);
        }

        if (childpid > 0)
            error = dup2(new_descriptors[1], STDOUT_FILENO);
        else
            error = dup2(new_descriptors[0], STDIN_FILENO);

        if (error == -1) {
            fprintf(stderr, "Could not dup pipes for iteration %d: %s\n",
                   process_count, strerror(errno));
            exit(1);
        }

        if ((close(new_descriptors[0]) == -1) || (close(new_descriptors[1]) == -1)) {
            fprintf(stderr, "Could not close extra descriptors %d: %s\n",
                   process_count, strerror(errno));
            exit(1);
        }

        if (childpid)
            execl("exec", "./exec", NULL);
    }

    char input_path[strlen(arguments[2]) + 5];
    sprintf(input_path, "DATA/%s", arguments[2]);
    FILE* input = fopen(input_path, "r");
    if (input == NULL) {
        fprintf(stderr, "Could not open input file: %s\n", strerror(errno));
        exit(1);
    }

    char output_path[strlen(arguments[3]) + 5];
    sprintf(output_path, "DATA/%s", arguments[3]);
    FILE* output = fopen(output_path, "w");
    if (output == NULL) {
        fprintf(stderr, "Could not create output file: %s\n", strerror(errno));
        exit(1);
    }

    int expressions = 0; //The number of expressions to be counted, stored in the first line of input
    int processed_expressions = 0;
    
    fgets(file_buffer, PIPE_SIZE, input);
    expressions = atoi(file_buffer);

    int i = 0;
    int line_number;
    
    while (i < expressions && i < max_processes) {
        i++;
        fgets(temp_buffer, PIPE_SIZE, input);
        sprintf(file_buffer, "%d: %s", i, temp_buffer);
        write_string(file_buffer);
    }

    while (processed_expressions < expressions) {
        read(0, file_buffer, 6);
        line_length = atoi(file_buffer);
        read(0, file_buffer, line_length + 1);

        if (isExpression(file_buffer) > 0) {
            write_string(file_buffer);
        } else {
            processed_expressions++;
            fprintf(output, "%s", file_buffer);
            if (processed_expressions + i <= expressions) {
                fgets(temp_buffer, PIPE_SIZE, input);
                line_number = i + processed_expressions;
                sprintf(file_buffer, "%d: %s", tmp, temp_buffer);
                write_string(file_buffer);
            }
        }

    }

    //Starting the chain of messages leading to killing all children
    write_string(kill_message);
    process_count = 0;	
    read(0, file_buffer, 6);
    line_length = atoi(file_buffer);
    read(0, file_buffer, line_length + 1);
    while (process_count < max_processes) {
        //Ensure each and every has used exit
        wait(0);
        process_count++;
    }

    //Closing input and output files
    fclose(input);
    fclose(output);

    return 0;
    exit(0);
}