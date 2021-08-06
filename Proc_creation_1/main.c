#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ERR_FORK (pid_t)-1
#define ERR_WAIT (pid_t)-1

#define MIN_ARG_COUNT 2
#define CHILD_PID 0

int check_status(int status)
{
    if (WIFEXITED(status))
    {
        printf("Exit status: %d\n", WEXITSTATUS(status));
        return 0;
    }

    if (WIFSIGNALED(status))
    {
        printf("Killed by signal: %d\n", WTERMSIG(status));
        return 0;
    }

    if (WIFSTOPPED(status))
    {
        printf("Stopped by signal: %d\n", WSTOPSIG(status));
        return 0;
    }

    if (WIFCONTINUED(status))
    {
        printf("Continued\n");
        return 0;
    }

    printf("Bad status\n");
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc < MIN_ARG_COUNT)
    {
        fprintf(stderr, "Expected: ./proc_creation program_name [arg1] [arg2] ... [null]\n");
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
        execvp(argv[1], &argv[1]);
        perror(argv[1]);
        exit(-1);
    }

    int status;
    pid_t child_pid = wait(&status);
    if (child_pid == ERR_WAIT)
    {
        perror(argv[0]);
        exit(-1);
    }
    check_status(status);

    return 0;
}
