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

void printDir()
{
    char cwd[2000];
    getcwd(cwd, sizeof(cwd));
    int len = strlen(home_dir);
    int l = strlen(cwd);
    if (l < len)
    {
        printf("%s", cwd);
    }
    else
    {

        int flag = 0, flag2 = 0;
        for (int i = 0; i < len; i++)
        {
            if (home_dir[i] != cwd[i])
            {
                flag = 1;
                break;
            }
        }
        if (l > len)
        {
            if (cwd[len] == '/')
            {
                flag2 = 1;
            }
        }
        if (flag == 0 && flag2 == 1)
        {
            printf("~");
            for (int i = len; i < l; i++)
            {
                printf("%c", cwd[i]);
            }
        }
        else if (flag == 0 && len == l)
        {
            printf("~");
        }
        else
        {
            printf("%s", cwd);
        }
    }
    printf(">");
}

void init_shell()
{
    printf("\033[1;32m");
    char *user = getenv("USER");
    printf("<%s@:", user);
    char hostname[300];
    gethostname(hostname, 300);
    printf("%s", hostname);
    printDir();
    printf("\033[0m");
}