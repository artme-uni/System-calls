#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>

#define ERR_FORK (pid_t)-1
#define ERR_WAIT (pid_t)-1

#define CHILD_PID 0
#define ARG_COUNT 2

int main(int argc, char *argv[])
{
    if(argc != ARG_COUNT)
    {
        perror("Expected: ./program [input.txt]");
        exit(-1);
    }

    pid_t current_pid = fork();

    if (current_pid == ERR_FORK)
    {
        perror(argv[0]);
        exit(-1);
    }

    if (current_pid == CHILD_PID)
    {
        execl("/bin/cat", "cat", argv[1], NULL);
        perror("Cannot exec cat");
        exit(-1);
    }

    pid_t child_pid = wait(NULL);

    if (child_pid == ERR_WAIT)
    {
        perror(argv[0]);
        exit(-1);
    }

    printf("\n\nHello from parent process with pid %d\n", current_pid);

    exit(EXIT_SUCCESS);
}