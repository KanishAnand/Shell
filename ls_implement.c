#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"
#include "main.h"

void ls_implement(char **args, int no_of_args)
{
    DIR *p;
    struct dirent *d;
    struct stat stats;

    if (no_of_args == 0 || strlen(args[1]) == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        args[1] = cwd;
    }

    if (args[1][0] == '-')
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        p = opendir(cwd);

        if (strcmp(args[1], "-l") == 0)
        {
            while (d = readdir(p))
            {
                char *nm = d->d_name;
                char s = nm[0];
                if (s != '.')
                {
                    printFileProperties(nm, stats);
                    printf("%s ", d->d_name);
                    printf("\n");
                }
            }
        }
        else if (strcmp(args[1], "-a") == 0)
        {
            while (d = readdir(p))
            {
                char *nm = d->d_name;
                char s = nm[0];
                printf("%s ", d->d_name);
                printf("\n");
            }
        }
        else if (strcmp(args[1], "-la") == 0 || strcmp(args[1], "-al") == 0)
        {
            while (d = readdir(p))
            {
                char *nm = d->d_name;
                char s = nm[0];
                printFileProperties(nm, stats);
                printf("%s ", d->d_name);
                printf("\n");
            }
        }
    }
    else
    {
        p = opendir(args[1]);
        if (p == NULL)
        {
            perror("Cannot find directory");
        }
        else
        {
            while (d = readdir(p))
            {
                char *nm = d->d_name;
                char s = nm[0];
                if (s != '.')
                {
                    printf("%s ", d->d_name);
                    printf("\n");
                }
            }
        }
    }
}

void printFileProperties(char *name, struct stat stats)
{
    struct stat buff;
    stat(name, &buff);
    struct tm *dt;
    printf((S_ISDIR(buff.st_mode)) ? "d" : "");
    printf((S_ISREG(buff.st_mode)) ? "-" : "");
    printf((S_ISLNK(buff.st_mode)) ? "l" : "");
    printf((buff.st_mode & S_IRUSR) ? "r" : "-");
    printf((buff.st_mode & S_IWUSR) ? "w" : "-");
    printf((buff.st_mode & S_IXUSR) ? "x" : "-");
    printf((buff.st_mode & S_IRGRP) ? "r" : "-");
    printf((buff.st_mode & S_IWGRP) ? "w" : "-");
    printf((buff.st_mode & S_IXGRP) ? "x" : "-");
    printf((buff.st_mode & S_IROTH) ? "r" : "-");
    printf((buff.st_mode & S_IWOTH) ? "w" : "-");
    printf((buff.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%ld ", buff.st_nlink);
    printf("%s ", getpwuid(buff.st_uid)->pw_name);
    printf("%s ", getgrgid(buff.st_gid)->gr_name);
    printf("%ld ", buff.st_size);
    char buffer[80];
    time_t timeStamp = buff.st_atime;
    dt = localtime(&timeStamp);
    strftime(buffer, 80, " %h %e ", dt);
    printf("%s", buffer);
    char tm[20];
    strftime(tm, 20, "%T", localtime(&(buff.st_mtime)));
    printf("%s ", tm);
    //printf("%s ", ctime(&buff.st_mtime));
    // File permissions
    // printf("\nFile access: ");
    // if (stats.st_mode & R_OK)
    //     printf("r");
    // if (stats.st_mode & W_OK)
    //     printf("w");
    // if (stats.st_mode & X_OK)
    //     printf("x ");

    // // File size
    // printf("%ld ", stats.st_size);

    // // Get file creation time in seconds and
    // // convert seconds to date and time format
    // dt = *(gmtime(&stats.st_ctime));
    // printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
    // // File modification time
    // dt = *(gmtime(&stats.st_mtime));
    // printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
}
