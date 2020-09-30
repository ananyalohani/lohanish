#include <stdio.h>

void cd(char *, char *, char *);
void echo(char *, char **);
void history(char *, int n, char *);

void cd(char *path, char *cur_dir, char *prev_dir)
{
    if (begins_with(path, "-P"))
    {
        // cd -P [PATHNAME]
        path = path + 2;
    }
    if (path[0] == '~') 
    {
        // cd ~
        strcpy(prev_dir, cur_dir);
        chdir(getenv("HOME"));
        getcwd(cur_dir, MAX_SIZE);
    }
    else if (path[0] != '/') 
    {
        // cd [RELATIVE PATHNAME]
        char *cur = (char *)calloc(MAX_SIZE, sizeof(char));
        getcwd(cur, MAX_SIZE);
        strcat(cur,"/");
        strcat(cur,path);
        int res = check_path_exists(cur);
        if (res == 1)
        {
            strcpy(prev_dir, cur_dir);

            char * new_path = (char *)calloc(MAX_SIZE, sizeof(char));
            realpath(cur, new_path);
            cur = new_path;

            strcpy(cur_dir, cur);
            chdir(cur);
        }
        else if (res == 0)
        {
            fprintf(stderr, "cd:- no such file or directory: %s\n", cur);
        } 
        else
        {
            fprintf(stderr, "cd:- failed to open directory: %s\n", cur);
        }
        
    } 
    else 
    {    
        int res = check_path_exists(path);
        if (res == 1)
        {
            strcpy(prev_dir, cur_dir);

            char * new_path = (char *)calloc(MAX_SIZE, sizeof(char));
            realpath(path, new_path);
            path = new_path;

            strcpy(cur_dir, path);
            chdir(path);
        }
        else if (res == 0)
        {
            fprintf(stderr, "cd:- no such file or directory: %s\n", path);
        } 
        else
        {
            fprintf(stderr, "cd:- failed to open directory: %s\n", path);
        }
    }
    return;
}

void echo(char *opt, char **args)
{
    int n_flag = 0;
    int E_flag = 0;
    int none_flag = 0;
    if (!strcmp(opt, "-n")) n_flag = 1;
    else if (!strcmp(opt, "-E")) E_flag = 1;
    else if (!strcmp(opt, "none")) none_flag = 1;
    else 
    {
        fprintf(stderr, "echo:- invalid command: %s \n", opt);
    }

    for (int i = 0; args[i]; i++)
    { 
        char *str = args[i];
        printf("%s ", str);
    }
    if(!n_flag)
        {
            printf("\n");
        }
}

void history(char *opt, int n, char *path)
{
    char h_file[MAX_SIZE];
    strcpy(h_file, path);
    strcat(h_file, "/text_files/history.txt");
    
    int no_of_lines = number_of_lines(h_file);
    
    if (strcmp(opt, "history") == 0) 
    {
        //print all commands
        FILE *fd = fopen(h_file, "r");
        if (fd == NULL) 
        {
            fprintf(stderr, "history:- the file '%s' could not be opened.\n", h_file);
            exit(1);
        }
        char *line = (char *)calloc(MAX_SIZE, sizeof(char));

        if (n == 0 || n >= no_of_lines)
        {
            for (int i = 0; i < no_of_lines; i++)
            {
                fgets(line, MAX_SIZE, fd);
                printf("%d %s", i+1, line);
            }
        }
        else 
        {
            n = no_of_lines - n;
            for (int i = 0; i < no_of_lines; i++)
            {
                fgets(line, MAX_SIZE, fd);
                if (i > n - 1) printf("%d %s", i+1, line);
            }

        }
        fclose(fd);
    }
    else if (strcmp(opt, "-c") == 0) 
    {
        //erase contents of history.txt
        FILE *fd = fopen(h_file, "w");
        if (fd == NULL) 
        {
            fprintf(stderr, "history:- the file '%s' could not be opened.\n", h_file);
            exit(1);
        }
        fclose(fd);
    }
    else
    {
        //delete line at offset: history -d <offset>
        char *line = (char *)calloc(MAX_SIZE, sizeof(char));
        char *offset = opt + 2;
        printf("%s\n", offset);
        int offs = 0;
        if(offset[0] == '-')
        {
            if (digits_only(offset + 1) == 0)
            {
                fprintf(stderr, "history:- invalid value of offset.\n");
                return;
            }
            offs = atoi(offset + 1);
            offs = no_of_lines - offs + 1;
        } 
        else 
        {
            if (digits_only(offset) == 0)
            {
                fprintf(stderr, "history:- invalid value of offset.\n");
                return;
            }
            offs = atoi(offset);
        }
        FILE *fd = fopen(h_file, "r");
        if (fd == NULL) 
        {
            fprintf(stderr, "history:- the file '%s' could not be opened.\n", h_file);
            exit(1);
        }

        char temp_file[MAX_SIZE];
        strcpy(temp_file, path);
        strcat(temp_file, "/temp.txt");

        FILE *temp = fopen(temp_file, "w");
        if (temp == NULL) 
        {
            fprintf(stderr, "history:- the file '%s' could not be opened.\n", temp_file);
            exit(1);
        }

        for (int i = 1; i <= no_of_lines; i++)
        {
            if (i == offs) 
            {
                fgets(line, MAX_SIZE, fd);
                continue;
            }
            fgets(line, MAX_SIZE, fd);
            fprintf(temp, "%s", line);
        }
        fclose(temp);
        fclose(fd);
        remove(h_file);
        rename(temp_file, h_file);
    }
}