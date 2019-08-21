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
#include "pinfo.h"
#include "system_commands.h"

void execute_command(char **args, int no_of_args)
{
    char cwd[1024];
    if (strcmp(args[0], "cd") == 0)
    {
        chdir(args[1]);
        printf("\n");
        init_shell();
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        printf("%s\n", args[1]);
        printf("\n");
        init_shell();
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[1024];
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
        printf("\n");
        init_shell();
    }
    else if (strcmp(args[0], "ls") == 0)
    {
        ls_implement(args, no_of_args);
        printf("\n");
        init_shell();
    }
    else if (strcmp(args[0], "pinfo") == 0)
    {
        pinfo(args, no_of_args);
        printf("\n");
        init_shell();
    }
    else
    {
        system_commands(args, no_of_args);
        init_shell();
    }
}
