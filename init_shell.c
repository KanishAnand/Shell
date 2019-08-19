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
    printf("~");
    for (int i = len; i < strlen(cwd); i++)
    {
        printf("%c", cwd[i]);
    }
    printf(">");
}

void init_shell()
{
    struct utsname buffer;
    printf("***");
    char *user = getenv("USER");
    printf("<%s@:", user);
    //printf("%s:",(&buffer)->sysname);
    printDir();
    // sleep(1);
}