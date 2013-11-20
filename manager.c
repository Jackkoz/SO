#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "so_lib.h"
#include <fcntl.h> 
#include <sys/wait.h>

int main(int argc, char* argv[]) {

    int     process_count;         /* number of this process (starting with 1) */
    int     childpid;              /* indicates process should spawn another   */
    int     max_processes;         /* total number of processes in ring        */
    int     fd[2];                 /* file descriptors returned by pipe        */
    int     error;                 /* return value from dup2 call              */

    char file_buffer[PIPE_SIZE];

    if (argc != 4) {
        fprintf (stderr, "Usage: %s max_processes, input_path, output_path\n", argv[0]);
        exit(1);
    }

    max_processes = atoi(argv[1]);

    /* check command line for a valid number of processes to generate */
    if (max_processes <= 0) {
        fprintf (stderr, "Max processes should be greater than 0.\n", argv[0]);
        exit(1);
    }

    if (pipe (fd) == -1) {
        perror("Could not create pipe");
        exit(1);
    }

    if ((dup2(fd[0], STDIN_FILENO) == -1) ||
            (dup2(fd[1], STDOUT_FILENO) == -1)) {
        perror("Could not dup pipes");
        exit(1);
    }

    if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
        perror("Could not close extra descriptors");
        exit(1);
    }

    for (process_count = 1; process_count <= max_processes; process_count++) {
        if (pipe (fd) == -1) {
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
            error = dup2(fd[1], STDOUT_FILENO);
        else
           error = dup2(fd[0], STDIN_FILENO);

        if (error == -1) {
            fprintf(stderr, "Could not dup pipes for iteration %d: %s\n",
                   process_count, strerror(errno));
            exit(1);
        }

        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
            fprintf(stderr, "Could not close extra descriptors %d: %s\n",
                   process_count, strerror(errno));
            exit(1);
        }

        if (childpid)
            execl("exec", "./exec", NULL);
    }

    FILE* input = fopen(argv[2], "r");
    if (input == NULL) {
        fprintf(stderr, "Could not open input file: %s\n", strerror(errno));
        exit(1);
    }

    FILE* output = fopen(argv[3], "w");
    if (output == NULL) {
        fprintf(stderr, "Could not create output file: %s\n", strerror(errno));
        exit(1);
    }

    int expressions = 0; //the number of expressions to be counted, stored in the first line of input
    int processed_expressions = 0;
    
    fgets(file_buffer, PIPE_SIZE, input);
    expressions = atoi(file_buffer);
    
    while (processed_expressions < expressions) {
        fgets(file_buffer, PIPE_SIZE, input);
        while (isExpression(file_buffer) > 0) { //The expression is still in need of computing
            write(1, file_buffer, strlen(file_buffer));
            read(0, file_buffer, PIPE_SIZE);
        }
        processed_expressions++;
        fprintf(output, "%d: %s", processed_expressions, file_buffer);
    }

    //Starting the chain of messages leading to killing all children
    write(1, kill_message, strlen(kill_message) + 1);
    process_count = 0;	
    read(0, file_buffer, PIPE_SIZE);
    while (process_count < max_processes) {
        //ensure each and every has used exit
        wait(0);
        process_count++;
    }

    //Closing input and output files
    fclose(input);
    fclose(output);

    return 0;
    exit(0);
}