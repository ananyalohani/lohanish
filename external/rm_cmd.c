#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *opt = argv[1];
    int none_flag = 0;
    int v_flag = 0;
    int i_flag = 0;
    if (strcmp(opt, "none") == 0) none_flag = 1;
    else if (strcmp(opt, "-v") == 0) v_flag = 1;
    else if (strcmp(opt, "-i") == 0) i_flag = 1;
    else 
    {
        fprintf(stderr, "rm: invalid command\n");
        return -1;
    }

    for (int i = 2; i < argc; i++)
    {
        char *pathname = argv[i];

        if (v_flag || none_flag)
        {
            // rm [FILEs]
            int res = unlink(pathname);
            if (res == -1)
            {
                switch (errno)
                {
                    case EACCES:
                        fprintf(stderr, "rm: cannot remove '%s': Permission denied\n", pathname);
                        break;
                    
                    case EISDIR:
                        fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", pathname);
                        break;

                    case ENOENT:
                        fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", pathname);
                        break;
                }
            }
            else 
            {
                // rm -v [FILEs]
                if (v_flag)
                    printf("removed %s\n", pathname);
            }
        }
        else if (i_flag)
        {
            // rm -i [FILEs]
            printf("rm: remove regular file: '%s'?\n", pathname);
            char *c = (char *)calloc(5, sizeof(char));
            fgets(c, 5, stdin);
            if (!strcmp(c, "y\n") || !strcmp(c, "Y\n"))
            {
                int res = unlink(pathname);
                if (res == -1)
                {
                    switch (errno)
                    {
                        case EACCES:
                            fprintf(stderr, "rm: cannot remove '%s': Permission denied\n", pathname);
                            break;
                        
                        case EISDIR:
                            fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", pathname);
                            break;

                        case ENOENT:
                            fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", pathname);
                            break;
                    }
                }
            }
        }
    }
    return 0;
}