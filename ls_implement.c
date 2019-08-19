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
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"
#include "main.h"

void ls_implement(char *command, char *arg, int no_of_args)
{
    DIR *p;
    struct dirent *d;
    struct stat stats;
    // printf("%d\n", no_of_args);
    // printf("%s\n", arg);
    // printf("%ld\n", strlen(arg));
    if (no_of_args == 0 || strlen(arg) == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        arg = cwd;
    }
    p = opendir(arg);
    if (p == NULL)
    {
        perror("Cannot find directory");
    }
    else
    {
        while (d = readdir(p))
        {
            if (stat(d->d_name, &stats) == 0)
            {
                printFileProperties(stats);
            }
            printf("%s ", d->d_name);
            printf("\n");
        }
        printf("\n");
    }
}

void printFileProperties(struct stat stats)
{
    struct tm dt;

    // File permissions
    // printf("\nFile access: ");
    if (stats.st_mode & R_OK)
        printf("r");
    if (stats.st_mode & W_OK)
        printf("w");
    if (stats.st_mode & X_OK)
        printf("x ");

    // File size
    printf("%ld ", stats.st_size);

    // Get file creation time in seconds and
    // convert seconds to date and time format
    dt = *(gmtime(&stats.st_ctime));
    printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
    // File modification time
    dt = *(gmtime(&stats.st_mtime));
    //printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
    //  dt.tm_hour, dt.tm_min, dt.tm_sec);
}
