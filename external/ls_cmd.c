#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#define BOLD_CYN    "\033[1m\033[36m"
#define RESET  "\033[0m"

int check_dir_error(DIR *);
int begins_with(char *, char *);

int main (int argc, char *argv[])
{
    char *cur_dir = argv[1];
    char *opt = argv[2];
    int m_flag = 0;
    int D_flag = 0;
    int none_flag = 0;
    if (strcmp(opt, "-m") == 0) m_flag = 1;
    else if (strcmp(opt, "-D") == 0) D_flag = 1;
    else if (strcmp(opt, "none") == 0) none_flag = 1;
    else 
    {
        fprintf (stderr, "ls:- invalid command\n");
        return -1;
    }

    DIR *dirp;     
    dirp = opendir(".");

    if(check_dir_error(dirp) == -1)
    {
        closedir(dirp);
        fprintf(stderr, "ls:- no such file or directory: %s\n", cur_dir);
        return -1;
    }

    if (m_flag == 1) 
    {
        // ls -m [DIRECTORYs]
        for (int i = 3; i < argc; i++)
        {
            char *dir = argv[i];
            int count = 0;
            DIR *dp;
            struct dirent *ep;

            dp = opendir(dir);
            if (check_dir_error(dp) == -1)
            {
                closedir(dp);
                fprintf(stderr, "ls:- no such file or directory: %s\n", dir);
                return -1;
            }
            while (ep = readdir(dp)) {
                count++;
            }
            closedir(dp);
            dp = opendir(dir);
            if (check_dir_error(dp) == -1)
            {
                closedir(dp);
                fprintf(stderr, "ls:- no such file or directory: %s\n", dir);
                return -1;
            }
            int j = 0;
            char *str = "";
            if(strcmp(dir, ".")) printf("\n%s%s:%s\n", BOLD_CYN, dir, RESET);
            while (ep = readdir(dp))
            {
                j++;
                str = ep->d_name;
                if (j == count) break;
                if (begins_with(str, ".")) continue;
                printf("%s,      ", str);
            }
            if (!begins_with(str, ".")) printf("%s\n", str);
            else printf("\n");
            
            closedir(dp);
        }
    }
    else if (D_flag)
    {
        // ls -D [DIRECTORYs]
        for(int i = 3; i < argc; i++)
        {    
            char *dir = argv[i];
            DIR *dp;
            struct dirent *ep;

            dp = opendir(dir);
            if (check_dir_error(dp) == -1)
            {
                closedir(dp);
                fprintf(stderr, "ls:- no such file or directory: %s\n", dir);
                return -1;
            }

            if (strcmp(dir, ".")) printf("\n%s%s:%s\n", BOLD_CYN, dir, RESET);
            while (ep = readdir(dp))
            {
                char *str = ep->d_name;
                if (begins_with(str, ".")) continue;
                printf("\"%s\"      ", str);
            }
            printf("\n");
            closedir(dp);
        }
    }
    else 
    {
        // ls [DIRECTORYs]
        for(int i = 3; i < argc; i++)
        {    
            char *dir = argv[i];
            DIR *dp;
            struct dirent *ep;

            dp = opendir(dir);
            if (check_dir_error(dp) == -1)
            {
                closedir(dp);
                fprintf(stderr, "ls:- no such file or directory: %s\n", dir);
                return -1;
            }

            if (strcmp(dir, ".")) printf("\n%s%s:%s\n", BOLD_CYN, dir, RESET);
            while (ep = readdir(dp))
            {
                char *str = ep->d_name;
                if (begins_with(str, ".")) continue;
                printf("%s      ", str);
            }
            printf("\n");
            closedir(dp);
        }   
    }
    closedir(dirp);
    return 0;
}

int check_dir_error(DIR *dp)
{
    if (dp == NULL) return -1;
    else return 0;
}

int begins_with(char *p, char *q)
{
    for (int i = 0; q[i]; i++)
    {
        if(p[i] != q[i]) return 0;
    }
    return 1;
}