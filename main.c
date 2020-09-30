#include "internal/utils.c"
#include "internal/internal.c"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RESET  "\033[0m"
#define BOLD_YLW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLD_BLU    "\033[1m\033[34m"      /* Bold Blue */
#define BOLD_MAG "\033[1m\033[35m"      /* Bold Magenta */
#define BOLD_CYN    "\033[1m\033[36m"      /* Bold Cyan */

void internal_command(char **, int, int);
void external_command(char **, int, int);

char cur_dir[MAX_SIZE];
char prev_dir[MAX_SIZE];
char start_cwd[MAX_SIZE];
//const char *ARR_CMD[] = {"cd", "echo", "history", "pwd", "exit", "ls", "cat", "date", "rm", "mkdir"};

int main(int argc, char *argv[])
{
    getcwd(cur_dir, MAX_SIZE);
    strcpy(prev_dir, cur_dir);
    strcpy(start_cwd, cur_dir);

    char h_file[MAX_SIZE];
    strcpy(h_file, start_cwd);
    strcat(h_file, "/text_files/history.txt"); 

    char *command = (char *)calloc(MAX_SIZE, sizeof(char));
    const char *shell_name = "lohanish";
    printf("%sWelcome to %s!\n\tCommands: cd, echo, history, pwd, exit, ls, cat, date, rm, mkdir\n", BOLD_MAG, shell_name);

    while (command != NULL)
    {
        bzero(command, MAX_SIZE);
        printf("%s%s: %s%s%s$ %s", BOLD_BLU, shell_name, BOLD_CYN, cur_dir, BOLD_YLW, RESET);

        fgets(command, MAX_SIZE, stdin);
        add_to_history(command, h_file);
        command[strlen(command) - 1] = '\0';

        if(command[0] == '\0' || command[0] == '\n')    continue;
        
        int ind = -1;
        int num = 0;
        char **w = is_valid(command, &ind, &num);

        if (ind == -1)
        {
            fprintf(stderr, "%s:- invalid command: %s\n", shell_name, command);
        } 
        else if (ind >= 0 && ind <= 4)
        {
            internal_command(w, ind, num);
        } 
        else if (ind >= 5 && ind <= 9)
        {
            external_command(w, ind, num);
        }
    }
    return 0;
}


void internal_command(char** w, int ind, int num) 
{
    char *command = w[0];
    char *option = (char *)calloc(MAX_SIZE, sizeof(char));

    if (ind == 0)
    {
        // cd
        if(num > 3)
        {
            fprintf(stderr, "cd:- too many arguments\n");
            return;
        }
        if (num > 1) option = w[1];
        if (option[0] == '/' || option[0] == '.' || isalpha(option[0])) 
        {
            // cd <argument>
            cd(option, cur_dir, prev_dir);
        } 
        else if (num == 1 || option[0] == '~') 
        {
            // cd
            cd("~", cur_dir, prev_dir);
        } 
        else if (!strcmp(option, "-P"))
        {
            // cd -P <path>
            if(num == 2) w[2] = "~";
            strcat(option, w[2]);
            cd(option, cur_dir, prev_dir);
        }
        else if (!strcmp(option, "--help"))
        {
            // cd --help
            if (num > 2)
            {
                fprintf(stderr, "cd:- too many arguments\n");
                return;
            }
            char cd_help[MAX_SIZE];
            strcpy(cd_help, start_cwd);
            strcat(cd_help, "/text_files/cd_help.txt");
            FILE *fd = fopen(cd_help, "r");
            if (fd == NULL)
            {
                fprintf(stderr, "cd:- could not open file: '%s'", cd_help);
                exit(-1);
            }
            char *s = (char *)calloc(MAX_SIZE, sizeof(char));
            while(fgets(s, MAX_SIZE, fd))
            {
                printf("%s", s);
            }
            printf("\n");
            fclose(fd);
            free(s);
        }
        else if (!strcmp(option, "-"))
        {
            // cd -
            swap(cur_dir, prev_dir);
            chdir(cur_dir);
            printf("%s\n", cur_dir);
        }
        else 
        {
            fprintf(stderr, "cd:- invalid command\n");
            return;
        }
    } 
    else if (ind == 1) 
    {
        // echo
        if(num < 2) 
        {
            fprintf(stderr, "echo:- missing argument\n");
            return;
        }
        option = "none";
        int i = 1;
        if(num > 1 && w[1][0] == '-') 
        {   
            // echo <option> <string>
            if(num < 3) 
            {
                fprintf(stderr, "echo:- missing argument\n");
                return;
            }
            i = 2;
            option = w[1];
        }
        remove_newline_char(option);
        echo(option, &w[i]);
    }
    else if (ind == 2) 
    {
        // history
        if(num > 3)
        {
            fprintf(stderr, "history:- too many arguments\n");
            return;
        }
        char *option = "none";
        int n = 0;
        if(num > 1) option = w[1];
        remove_newline_char(option);

        if (!strcmp(option, "-c"))
        {
            // history -c
            if(num > 2)
            {
                fprintf(stderr, "history:- too many arguments\n");
                return;
            }
            history(option, n, start_cwd);
        } 
        else if (!strcmp(option, "-d"))
        {
            // history -d <offset>
            if(num < 3)
            {
                fprintf(stderr, "history:- missing argument\n");
                return;
            }
            strcat(option, w[2]);
            history(option, n, start_cwd);
        }
        else if (digits_only(option))
        {
            n = atoi(option);
            option = "none";
            history(w[0], n, start_cwd);
        }
        else if (!strcmp(option, "none"))
        {
            // history
            history(w[0], n, start_cwd);
        }
        else 
        {
            fprintf(stderr, "history:- invalid command\n");
            return;
        }
    } 
    else if (ind == 3) 
    {
        // pwd
        char *option = "none";
        if(num > 2)
        {
            fprintf(stderr, "pwd:- too many arguments\n");
            return;
        }
        if(num > 1) option = w[1];
        remove_newline_char(option);

        if (!strcmp(option, "-P") || !strcmp(option, "none"))
        {
            // pwd -P
            char *path = (char *)calloc(MAX_SIZE, sizeof(char));
            char *new_path = (char *)calloc(MAX_SIZE, sizeof(char));
            getcwd(path, MAX_SIZE);
            realpath(path, new_path);
            printf("%s\n", new_path);
        } 
        else if (!strcmp(option, "--help"))
        {
            // pwd --help
            char pwd_help[MAX_SIZE];
            strcpy(pwd_help, start_cwd);
            strcat(pwd_help, "/text_files/pwd_help.txt");
            FILE *fd = fopen(pwd_help, "r");
            if (fd == NULL)
            {
                fprintf(stderr, "pwd:- could not open file: '%s'", pwd_help);
                exit(-1);
            }
            char *s = (char *)calloc(MAX_SIZE, sizeof(char));
            while(fgets(s, MAX_SIZE, fd))
            {
                printf("%s", s);
            }
            printf("\n");
            fclose(fd);
            free(s);
        }
        else
        {
            fprintf(stderr, "pwd:- invalid command\n");
            return;
        }
    } 
    else
    {
        // exit
        if (num > 2)
        {
            fprintf(stderr, "exit:- too many arguments\n");
            return;
        }
        else if (num == 2)
        {
            // exit <status>
            int res = digits_only(w[1]);
            if(res != 1)
            {
                fprintf(stderr, "exit:- numeric argument required");
                return;
            }
            else 
            {
                printf("exit\n");
                exit(atoi(w[1]));
            }
        }
        else 
        {
            // exit
            exit(0);
        }
    }    
}

void external_command(char **w, int ind, int num)
{
    char *command = w[0];
    char *option = (char *)calloc(MAX_SIZE, sizeof(char));
    char **args;

    pid_t pid = fork();

    if(pid == 0)
    {
        if (ind == 5)
        {
            // ls
            option = "none";
            int i = 1;
            if (num > 1 && w[1][0] == '-')
            {
                i = 2;
                option = w[1];
            }

            strcat(start_cwd, "/external/ls_cmd");
            if (num == i) 
            {
                args = malloc(5 * sizeof(*args));
                args[0] = start_cwd;
                args[1] = cur_dir;
                args[2] = option;
                args[3] = ".";
                args[4] = NULL;
            }
            else 
            {
                args = malloc((4 + (num - i)) * sizeof(*args));
                memmove(&args[3], &w[i], sizeof(*args) * (num - i));
                args[0] = start_cwd;
                args[1] = cur_dir;
                args[2] = option;
                args[num -i + 3] = NULL;
            }
        }
        else if (ind == 6)
        {
            // cat
            option = "none";
            int i = 1;
            if (num > 1)
            {
                if (!strcmp(w[1], "-"))
                {
                    option = "none";
                    i = 1;
                }
                else if (w[1][0] == '-')
                {
                    i = 2;
                    option = w[1]; 
                }
            }

            strcat(start_cwd, "/external/cat_cmd");
            if (num == i)
            {
                args = malloc(4 * sizeof(*args));
                args[0] = start_cwd;
                args[1] = option;
                args[2] = "-";
                args[3] = NULL;
            }
            else 
            {
                args = malloc((3 + (num - i)) * sizeof(*args));
                memmove(&args[2], &w[i], sizeof(*args) * (num - i));
                args[0] = start_cwd;
                args[1] = option;
                args[num -i + 2] = NULL;
            }
        }
        else if (ind == 7)
        {
            // date
            if(num > 3)
            {
                fprintf(stderr, "date:- too many arguments\n");
                return;
            }
            option = "none";
            char *format = "+%c";
            if(num > 1)
            {
                if(w[1][0] == '-') 
                {
                    option = w[1];
                }
                else 
                {
                    format = w[1];
                }

                if(num > 2)
                {
                    format = w[2];
                } 
            }
            if(format[0] != '+')
            {
                fprintf(stderr, "date:- invalid format\n");
                return;
            }
            format = format + 1;
            
            strcat(start_cwd, "/external/date_cmd");
            args = malloc(4 * sizeof(*args));
            args[0] = start_cwd;
            args[1] = option;
            args[2] = format;
            args[3] = NULL;
        }
        else if (ind == 8)
        {
            // rm
            option = "none";
            int i = 1;
            if (num < 2)
            {
                fprintf(stderr, "rm:- missing arguments\n");
                return;
            }
            if(num > 1 && w[1][0] == '-') 
            {
                if (num < 3)
                {
                    fprintf(stderr, "rm:- missing arguments\n");
                    return;
                }
                i = 2;
                option = w[1];
            }

            strcat(start_cwd, "/external/rm_cmd");
            args = malloc((3 + (num - i)) * sizeof(*args));
            memmove(&args[2], &w[i], sizeof(*args) * (num - i));
            args[0] = start_cwd;
            args[1] = option;
            args[num -i + 2] = NULL;  
        }
        else 
        {
            // mkdir
            option = "none";
            int i = 1;
            if (num < 2)
            {
                fprintf(stderr, "mkdir:- missing arguments\n");
                return;
            }
            if(num > 1 && w[1][0] == '-') 
            {
                 if (num < 3)
                {
                    fprintf(stderr, "mkdir:- missing arguments\n");
                    return;
                }
                i = 2;
                option = w[1];
            }

            strcat(start_cwd, "/external/mkdir_cmd");
            args = malloc((3 + (num - i)) * sizeof(*args));
            memmove(&args[2], &w[i], sizeof(*args) * (num - i));
            args[0] = start_cwd;
            args[1] = option;
            args[num -i + 2] = NULL;   
        }

        int res = execv(args[0], args) == -1;
        char *error = (char *) calloc(MAX_SIZE, sizeof(char));
        snprintf(error, MAX_SIZE, "%s", cur_dir);
        perror(error);

        free(error);
        exit(res);
    } 
    else 
    {
        wait(NULL);
        return;
    }
    
}

