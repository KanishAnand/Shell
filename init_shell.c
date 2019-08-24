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
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    int len = strlen(home_dir);
    if (strlen(cwd) < len)
    {
        printf("%s", cwd);
    }
    else
    {
        printf("~");
        for (int i = len; i < strlen(cwd); i++)
        {
            printf("%c", cwd[i]);
        }
    }
    printf(">");
}

void init_shell()
{
    printf("\033[1;32m");
    struct utsname buffer;
    char *user = getenv("USER");
    printf("<%s@:", user);
    char hostname[300];
    gethostname(hostname, 300);
    printf("%s", hostname);
    //printf("%s:",(&buffer)->sysname);
    printDir();
    // sleep(1);
    printf("\033[0m");
}