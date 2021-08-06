#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/mman.h>
#include <stdbool.h>

#define SYS_CALL_ERR -1
#define SYS_ERR_SELECT (off_t)-1
#define SYS_ERR_WRITE -1
#define SYS_FREE_FD_SET 0

#define CMD_EXIT 0
#define CMD_NOT_NUM -9

#define ERR_MEMORY -3
#define ERR_SCAN -4
#define ERR_PRINT -5
#define ERR_TIMEOUT -6
#define ERR_FREE -7
#define ERR_MAPPING -8

#define TABLE_SIZE 128
#define BUFF_SIZE 1024

#define WAIT_TIME 5
#define FILE_OFFSET 0

typedef struct TableElement
{
    int offset;
    int length;
} TableElement;

int add_element(TableElement *table, int line_length, int *line_number, int *max_count)
{
    if (*line_number >= *max_count)
    {
        table = realloc(table, (sizeof(TableElement) * (*max_count + TABLE_SIZE)));
        if (table == NULL)
        {
            fprintf(stderr, "Cannot reallocate memory in Table");
            return ERR_MEMORY;
        }
        *max_count += TABLE_SIZE;
    }

    if (*line_number == 0)
    {
        table[*line_number].offset = 0;
    } else
    {
        table[*line_number].offset = table[*line_number - 1].offset + table[*line_number - 1].length;
    }

    table[*line_number].length = line_length;
    ++*line_number;

    return 0;
}

int make_table(const char *input_file, int file_size, TableElement *table, int *line_count)
{
    int max_count = TABLE_SIZE;
    int line_length = 0;

    int index;
    for (index = 0; index < file_size; ++index)
    {
        line_length++;

        if (input_file[index] != '\n')
        {
            continue;
        }

        int err = add_element(table, line_length, line_count, &max_count);
        if (err == ERR_MEMORY)
        {
            return ERR_MEMORY;
        }

        line_length = 0;
    }

    if (input_file[index] != '\n')
    {
        int err = add_element(table, line_length, line_count, &max_count);
        if (err == ERR_MEMORY)
        {
            return ERR_MEMORY;
        }
    }

    return 0;
}

int is_number(char *number_c)
{
    for (char *str = number_c; *str != '\0'; str++)
    {
        if (!(isdigit((int) *str)))
        {
            return false;
        }
    }

    return true;
}

int wait_input(int time, char *input_text)
{
    struct timeval tv;
    tv.tv_sec = time;
    tv.tv_usec = 0;

    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(STDIN_FILENO, &read_fd);

    int status = select(STDIN_FILENO + 1, &read_fd, NULL, NULL, &tv);

    if (status == SYS_ERR_SELECT)
    {
        perror("Cannot set waiting time");
        return ERR_TIMEOUT;
    }

    int input_status = FD_ISSET(STDIN_FILENO, &read_fd);

    if (input_status == SYS_FREE_FD_SET)
    {
        printf("\nInput time limit : %d sec\n\n", time);
        printf("%s", input_text);
        return ERR_TIMEOUT;
    }

    return 0;
}

int read_number(int *line_number, char *input_text)
{
    int err_write = write(STDOUT_FILENO, "Enter line number: ", 19);
    if (err_write == SYS_ERR_WRITE)
    {
        perror("Cannot write init message");
        return ERR_PRINT;
    }

    int status = wait_input(WAIT_TIME, input_text);
    if (status == ERR_TIMEOUT)
    {
        return ERR_TIMEOUT;
    }

    char read_number[BUFF_SIZE] = {};

    int err_read = read(STDIN_FILENO, read_number, BUFF_SIZE);
    if (err_read == SYS_CALL_ERR)
    {
        perror("Cannot scan input line number");
        return ERR_SCAN;
    }

    read_number[strlen(read_number) - 1] = '\0';

    int condition = is_number(read_number);

    if (condition == false)
    {
        printf("INCORRECT INPUT: '%s' is not a number\n\n", read_number);
        return CMD_NOT_NUM;
    }

    *line_number = atoi(read_number);

    return 0;
}

int print_string(char *input_file, int file_size, TableElement *table, int line_count)
{
    int number_line = 1;
    while (number_line != CMD_EXIT)
    {
        int err_code = read_number(&number_line, input_file);

        if (err_code == CMD_NOT_NUM || number_line == CMD_EXIT)
            continue;

        if (err_code < 0)
        {
            return ERR_SCAN;
        }

        if ((number_line < 0) || (number_line > line_count))
        {
            printf("INCORRECT INPUT: Line number must be > 0 and < %d\n\n", line_count + 1);
            continue;
        }

        for (int i = table[number_line - 1].offset;
             i < table[number_line - 1].offset + table[number_line - 1].length; ++i)
        {
            printf("%c", input_file[i]);
        }
        printf("\n");
    }

    return 0;
}

int free_resources(TableElement *table, char *input_file, int file_size)
{
    if (table != NULL)
    {
        free(table);
    }

    if (input_file != MAP_FAILED)
    {
        int err_code = munmap(input_file, file_size);
        if (err_code == SYS_CALL_ERR)
        {
            perror("Cannot unmap input file");
            return ERR_FREE;
        }
    }

    return 0;
}

void line_searching(TableElement *table, char *input_file, int file_size)
{
    int line_count = 0;

    int err_mt = make_table(input_file, file_size, table, &line_count);
    if (err_mt < 0)
    {
        int err_free = free_resources(table, input_file, file_size);
        if (err_free == ERR_FREE)
        {
            exit(ERR_FREE);
        }
        exit(err_mt);
    }

    int err_ps = print_string(input_file, file_size, table, line_count);
    if (err_ps < 0)
    {
        int err_free = free_resources(table, input_file, file_size);
        if (err_free == ERR_FREE)
        {
            exit(ERR_FREE);
        }
        exit(err_ps);
    }
}

int create_mapping(char **input_file, char *file_name, int *file_size)
{
    int file_handle;

    file_handle = open(file_name, O_RDONLY);
    if (file_handle == SYS_CALL_ERR)
    {
        perror("Cannot open input file for making table");
        return ERR_MAPPING;
    }

    *file_size = lseek(file_handle, 0, SEEK_END);
    if (*file_size == SYS_ERR_SELECT)
    {
        perror("Cannot define file size");
        return ERR_MAPPING;
    }

    *input_file = (char *) mmap(0, *file_size - FILE_OFFSET, PROT_READ, MAP_PRIVATE, file_handle, FILE_OFFSET);
    if (input_file == MAP_FAILED)
    {
        perror("Cannot map input file for making table");
        return ERR_MAPPING;
    }

    int err_code = close(file_handle);
    if (err_code == SYS_CALL_ERR)
    {
        perror("Cannot close input file");
        return ERR_FREE;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Expected: ./program input.txt\n");
        exit(-1);
    }

    TableElement *table = NULL;
    char *input_file = MAP_FAILED;
    int file_size = 0;

    table = (TableElement *) malloc(sizeof(TableElement) * TABLE_SIZE);
    if (table == NULL)
    {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(-1);
    }

    int err_code = create_mapping(&input_file, argv[1], &file_size);
    if (err_code == ERR_MAPPING)
    {
        free_resources(table, input_file, file_size);
    }

    line_searching(table, input_file, file_size);

    free_resources(table, input_file, file_size);
    return 0;
}