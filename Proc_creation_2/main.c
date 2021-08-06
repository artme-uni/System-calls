#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int execvpe(char *file_name, char **argv, char **envp)
{
    environ = envp;
    execvp(file_name, argv);

    perror(argv[0]);
    return -1;
}

int main()
{
    char *argv[] = {"env", "First", "Second", NULL};
    char *envp[] = {"PATH=/Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug", "USER=Artem", "HOME=/home", NULL};

    execvpe(argv[0], argv, envp);
    exit(-1);
}