#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *opt = argv[1];
    char *format = (char *)calloc(100, sizeof(char));
    strcpy(format, argv[2]);
    int u_flag = 0;
    int R_flag = 0;
    int none_flag = 0;
    if (strcmp(opt, "-u") == 0) u_flag = 1;
    else if (strcmp(opt, "-R") == 0)
    {
        R_flag = 1;
        strcpy(format, "%a, %d %b %Y %T %z");
    }
    else if (strcmp(opt, "none") == 0) none_flag = 1;
    else 
    {
        fprintf(stderr, "date:- invalid command\n");
        return -1;
    }
    
    if (u_flag)
    {
        // date -u [+FORMAT]
        time_t t = time(NULL);
        struct tm *gmt;
        time(&t);
        gmt = gmtime(&t);
        char s[64];
        assert(strftime(s, sizeof(s), format, gmt));
        printf("%s\n", s);
    }
    else if (R_flag || none_flag)
    {
        // date -R or date [+FORMAT]
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char s[64];
        assert(strftime(s, sizeof(s), format, tm));
        printf("%s\n", s);
    }
    return 0;
}