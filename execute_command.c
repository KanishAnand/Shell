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

void execute_command(char *command, char *arg, int no_of_args)
{
    char cwd[1024];
    if (strcmp(command, "cd") == 0)
    {
        chdir(arg);
        init_shell();
    }
    else if (strcmp(command, "echo") == 0)
    {
        printf("%s\n", arg);
        init_shell();
    }
    else if (strcmp(command, "pwd") == 0)
    {
        char cwd[1024];
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
        init_shell();
    }
    else if (strcmp(command, "ls") == 0)
    {
        ls_implement(command, arg, no_of_args);
        init_shell();
    }
}
