#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *opt = argv[1];
    int p_flag = 0;
    int v_flag = 0;
    int none_flag = 0;
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    if (!strcmp(opt, "-p")) p_flag = 1;
    else if (!strcmp(opt, "-v")) v_flag = 1;
    else if (!strcmp(opt, "none")) none_flag = 1;
    else
    {
        fprintf(stderr, "mkdir:- invalid command\n");
        return -1;
    }
    for (int i = 2; i < argc; i++)
    {
        char *path = argv[i];
        // mkdir [DIRECTORYs]
        int res = mkdir(path, mode);
        if (res == -1)
        {
            switch (errno)
            {
                case EACCES:
                    fprintf(stderr, "mkdir:- cannot create directory '%s': Permission denied\n", path);
                    break;

                case EEXIST:
                {
                    // mkdir -p [DIRECTORYs]
                    if (p_flag) break;
                    fprintf(stderr, "mkdir:- cannot create directory '%s': File exists\n", path);
                    break;
                }
                case ENOENT:
                    fprintf(stderr, "mkdir:- cannot create directory '%s': No such file or directory\n", path);
            }
        }
        else 
        {
            // mkdir -v [DIRECTORYs]
            if (v_flag)
                printf("created directory '%s'\n", path);
        }
    }
}

