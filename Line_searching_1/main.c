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
#define ERR_TIMEOUT -6

#define NOT_NUM -9

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
int wait_time(int time)
{
    struct timeval tv;
    tv.tv_sec = time;
    tv.tv_usec = 0; //микросекунды

    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(STDIN_FILENO, &read_fd);

    int status = select(STDIN_FILENO + 1, &read_fd, NULL, NULL, &tv);

    if(status == -1)
    {
        perror("Cannot set waiting time");
        return status;
    }

    return FD_ISSET(STDIN_FILENO, &read_fd);
}

int print_file(int handle)
{
    int err_lseek = lseek(handle, 0, SEEK_SET);
    if (err_lseek == -1)
    {
        perror("Cannot print file");
        return ERR_READFILE;
    }

    char buff[BUFF_SIZE];
    while(1)
    {
        int chars_count = read(handle, buff, BUFF_SIZE -1);
        if (chars_count == -1)
        {
            perror("Cannot print file");
            return ERR_READFILE;
        }

        if(chars_count == 0)
            return 0;

        buff[chars_count] = '\0';
        printf("%s", buff);
    }

}

int read_number(int *number, int handle)
{
    int err_write = write(STDOUT_FILENO, "Enter line number: ", 19);
    if (err_write == -1)
    {
        perror("Cannot write init message");
        return ERR_PRINT;
    }

    char *line_number = (char *) calloc(BUFF_SIZE, sizeof(char));

    int status = wait_time(5);
    if(status == -1)
    {
        return ERR_TIMEOUT;
    }

    if(status == 0)
    {
        printf("\nInput time limit : 5 sec\n\n");
        print_file(handle);
        return ERR_TIMEOUT;
    }

    int err_read = read(STDIN_FILENO, line_number, BUFF_SIZE);
    if (err_read == -1)
    {
        perror("Cannot scan input line number");
        return ERR_SCAN;
    }

    line_number[strlen(line_number) - 1] = '\0';

    int condition = is_number(line_number);

    if (condition == 0)
    {
        printf("INCORRECT INPUT: '%s' is not a number\n\n", line_number);
        return NOT_NUM;
    }

    *number = atoi(line_number);

    free(line_number);

    return 0;
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
        int err_code = read_number(&number_line, handle);

        if (err_code == NOT_NUM || number_line == EXIT_CMD)
            continue;

        if (err_code < 0)
        {
            return err_code;
        }

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