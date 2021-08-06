#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
    printf("Arguments:\n");
    for (int n = 0; n < argc; n++)
        printf("\t%2d: '%s'\n", n, argv[n]);

    printf("\nEnvironmental variables:\n");
    for (char **p = environ; *p != 0; p++)
        printf("\t%s\n", *p);

    return 0;
}