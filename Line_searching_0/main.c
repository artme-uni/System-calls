#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#define EXIT_CMD 0
#define ERR_OPENFILE -1
#define ERR_READFILE -2
#define ERR_MEMORY -3
#define ERR_SCAN -4
#define ERR_PRINT -5

#define NOT_NUM -6

#define TABLE_SIZE 128
#define BUFF_SIZE 1024

typedef struct TableElement
{
    int offset;
    int length;
} TableElement;


int add_element(TableElement *table, int offset, int line_length, int *line_count, int *max_count)
{
    if (*line_count > 0)
    {
        table[*line_count].offset = table[*line_count - 1].offset + table[*line_count - 1].length;
    }

    if (*line_count == 0)
    {
        table[*line_count].offset = 0;
    }

    table[*line_count].length = line_length;
    ++*line_count;

    if (*line_count >= *max_count)
    {
        table = realloc(table, (sizeof(TableElement) * (*max_count + TABLE_SIZE)));
        if (table == NULL)
        {
            fprintf(stderr, "Cannot reallocate memory in Table");
            return ERR_MEMORY;
        }
        *max_count += TABLE_SIZE;
    }

    return 0;

    table[*line_count + 1].offset = offset;
    table[*line_count].length = line_length;
    ++*line_count;


}


int make_table(char *name, TableElement *table, int *line_count)
{
    int handle = open(name, O_RDONLY);
    if (handle == -1)
    {
        perror("Cannot open input file for making table");
        return ERR_OPENFILE;
    }

    int max_count = TABLE_SIZE;
    int offset = 0, line_length = 0;
    char symbol;
    ssize_t byte_count = 0;

    do
    {
        byte_count = read(handle, &symbol, 1);
        if (byte_count == -1)
        {
            perror("Cannot read input file for making table");
            return ERR_READFILE;
        }
        if (byte_count == 0)
        {
            continue;
        }

        ++offset;
        ++line_length;

        if (symbol != '\n')
        {
            continue;
        }

        int err = add_element(table, offset, line_length, line_count, &max_count);
        if (err == ERR_MEMORY)
        {
            return err;
        }
        line_length = 0;

    } while (byte_count > 0);

    if (symbol != '\n')
    {
        int err = add_element(table, offset, line_length, line_count, &max_count);
        if (err == ERR_MEMORY)
        {
            return err;
        }
    }

    close(handle);
    return 0;
}

int is_number(char *number_c)
{
    for (char *str = number_c; *str != '\0'; str++)
    {
        if (!(isdigit((int) *str)))
        {
            return 0;
        }
    }

    return 1;
}

int scan_number(int *number)
{
    int err = 0;

    int err_write = write(STDOUT_FILENO, "Enter line number: ", 19);
    if (err_write == -1)
    {
        perror("Cannot write init message");
        err = ERR_PRINT;
    }

    char *line_number = (char *) calloc(BUFF_SIZE, sizeof(char));

    int err_read = read(STDIN_FILENO, line_number, BUFF_SIZE);
    if (err_read == -1)
    {
        perror("Cannot scan input line number");
        err = ERR_SCAN;
    }

    line_number[strlen(line_number) - 1] = '\0';

    int condition = is_number(line_number);

    if (condition == 0)
    {
        printf("INCORRECT INPUT: '%s' is not a number\n\n", line_number);
        err = NOT_NUM;
    }

    *number = atoi(line_number);

    free(line_number);
    return err;
}

int print_string(char *name, TableElement *table, int line_count)
{
    int handle = open(name, O_RDONLY);
    if (handle == -1)
    {
        perror("Cannot open input file for printing required line");
        return ERR_OPENFILE;
    }

    int number_line = 1;

    while (number_line != EXIT_CMD)
    {
        int err_code = scan_number(&number_line);

        if (err_code < 0)
        {
            return err_code;
        }

        if (number_line == EXIT_CMD)
            continue;

        if ((number_line < 0) || (number_line > line_count))
        {
            printf("INCORRECT INPUT: Line number must be > 0 and < %d\n\n", line_count + 1);
            continue;
        }

        int err_lseek = lseek(handle, table[number_line - 1].offset, SEEK_SET);
        if (err_lseek == -1)
        {
            perror("Cannot print required line");
            return ERR_READFILE;
        }

        char buffer[table[number_line - 1].length];
        int err_read = read(handle, buffer, table[number_line - 1].length);
        if (err_read == -1)
        {
            perror("Cannot print required line");
            return ERR_READFILE;
        }

        buffer[table[number_line - 1].length] = '\0';
        printf("%s\n", buffer);
    }
    close(handle);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Expected: ./program input.txt\n");
        exit(-1);
    }

    int line_count = 0;

    TableElement *table = (TableElement *) malloc(sizeof(TableElement) * TABLE_SIZE);
    if (table == NULL)
    {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(-1);
    }

    int err_mt = make_table(argv[1], table, &line_count);
    if (err_mt < 0)
    {
        free(table);
        exit(-1);
    }

    int err_ps = print_string(argv[1], table, line_count);
    if (err_ps < 0)
    {
        free(table);
        exit(-1);
    }

    return 0;
}

