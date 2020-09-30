#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 500

void input_loop(int, int);

int main(int argc, char *argv[])
{
    char *opt = argv[1];
    int n_flag = 0;
    int E_flag = 0;
    int none_flag = 0;
    if (strcmp(opt, "-n") == 0) n_flag = 1;
    else if (strcmp(opt, "-E") == 0) E_flag = 1;
    else if(strcmp(opt, "none") == 0) none_flag = 1;
    else 
    {
        fprintf(stderr, "cat:- invalid command\n");
        return -1;
    }
    if (!strcmp(argv[2], "-"))
    {   
        // cat [OPTION] [-]
        input_loop(n_flag, E_flag);
    } 
    else
    {
        int j = 1;
        for (int i = 2; i < argc; i++)
        {
            FILE *fd = fopen(argv[i], "r");
            if (fd == NULL)
            {
                fprintf(stderr, "cat:- could not open file: '%s'", argv[i]);
                return -1;
            }
            char *s = (char *)calloc(MAX_SIZE, sizeof(char));
            if (n_flag)
            {
                // cat -n [FILEs]
                while(fgets(s, MAX_SIZE, fd)) 
                {
                    printf("   %d  %s", j++, s);
                }
            }
            else if (E_flag)
            {
                // cat -E [FILEs]
                while(fgets(s, MAX_SIZE, fd))
                {
                    char *str = strtok(s, "\n");
                    if  (!str) continue;
                    printf("%s$\n", s);
                }
            } 
            else {
                // cat [FILEs]
                while(fgets(s, MAX_SIZE, fd))
                {
                    printf("%s", s);
                }
            }

            if (!E_flag)
            {
                printf("\n");
            }
            
            fclose(fd);
        }
    }
    return 0;
}

void input_loop(int n_flag, int E_flag)
{
    char *str = (char *)calloc(MAX_SIZE, sizeof(char));
    if (n_flag)
    {
        // cat -n [-]
        int j = 1;
        while (1)
        {
            str = fgets(str, MAX_SIZE, stdin);
            if (feof(stdin)) break;
            printf("\t%d  %s",j++, str);
        }
        printf("\n");
    }
    else if(E_flag)
    {
        // cat -E [-]
        while (1)
        {
            str = fgets(str, MAX_SIZE, stdin);
            if (feof(stdin)) break;
            str[strlen(str) - 1] = '\0';
            printf("%s$\n", str);
        }
    }
    else 
    {
        // cat [-]
        while (1)
        {
            str = fgets(str, MAX_SIZE, stdin);
            if (feof(stdin)) break;
            printf("%s", str);
        }
        printf("\n");
    }
}