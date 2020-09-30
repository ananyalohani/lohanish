#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <wordexp.h>

#define MAX_SIZE 500

char **is_valid(char *, int *, int *);
int command_in(char *, int *);
void remove_newline_char(char *);
int digits_only(char *);
int begins_with(char *, char *);
int number_of_lines(char *);
void add_to_history(char *, char *);
int check_path_exists(char *);
void swap(char *, char *);

const char *ARR_CMD[] = {"cd", "echo", "history", "pwd", "exit", "ls", "cat", "date", "rm", "mkdir"};
const char *history_file;

char** is_valid(char *command, int *ind, int *num)
{
    wordexp_t p;
    wordexp(command, &p, 0);
    char **w = p.we_wordv;
    int res = command_in(w[0], ind);
    if(res == -1) 
    {
        *ind = -1;
        return NULL;
    }
    else 
    {
        *num = p.we_wordc;
        return w;
    }
}

void swap(char *s1, char *s2)
{
    char temp[MAX_SIZE]= "";
    strcpy(temp, s1);
    strcpy(s1, s2);
    strcpy(s2, temp);
}

int check_path_exists(char *path)
{
    DIR* dir = opendir(path);
    if (dir) {
        /* Directory exists. */
        return 1;
        closedir(dir);
    } else if (ENOENT == errno) {
        /* Directory does not exist. */
        return 0;
    } else {
        /* opendir() failed for some other reason. */
        return -1;
    }
}


int command_in(char *cmd, int *ind) 
{
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(cmd, ARR_CMD[i]) == 0)
        {
            *ind = i;
            return 0;
        }
    }
    return -1;
}

void add_to_history(char *cmd, char *filename)
{
    history_file = filename;
    FILE *fd = fopen(filename, "a");
    if (fd == NULL) 
    {
        fprintf(stderr, "history:- the file '%s' could not be opened.\n", filename);
        exit(1);
    }
    fprintf(fd, "%s", cmd);
    fclose(fd);
}

int begins_with(char *p, char *q)
{
    for (int i = 0; q[i]; i++)
    {
        if(p[i] != q[i]) return 0;
    }
    return 1;
}

void remove_newline_char(char *str) 
{
    char *newline = strchr(str, '\n');
    if (newline) 
    {
        *newline = '\0';
    } 
}

int number_of_lines(char *filename) 
{
    FILE *fd= fopen(filename, "r");
    if (fd == NULL) 
    {
        printf("history:- the file '%s' could not be opened.\n", filename);
        exit(1);
    }
    char c = getc(fd);
    int count = 0;
    while (c != EOF)
    {
        if (c == '\n')
        {
            count++;
        } 
        c = getc(fd);
    }
    fclose(fd);
    return count;
}

int digits_only(char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}
