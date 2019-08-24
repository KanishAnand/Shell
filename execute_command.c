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
#include "history.h"
#include "nightswatch.h"
#include "system_commands.h"

void execute_command(char **args, int no_of_args)
{
    // char cwd[1024];
    // printf("%s  %s\n", args[0], args[1]);
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL || strlen(args[1]) == 0 || strcmp(args[1], "~") == 0)
        {
            args[1] = home_dir;
        }
        char *newpath = (char *)malloc(sizeof(char) * 300);
        strcpy(newpath, home_dir);

        int l = strlen(home_dir);
        l--;
        if (args[1][0] == '~')
        {
            int i;
            for (i = 1; i < strlen(args[1]); i++)
            {
                newpath[l + i] = args[1][i];
            }
            // put null character at end of string
            newpath[l + i] = '\0';
            // printf("%s\n", newpath);
            if (chdir(newpath) == -1)
            {
                perror("cd");

                // printf("%s", args[1]);
            }
        }
        else
        {
            if (chdir(args[1]) == -1)
            {
                perror("cd");
                // printf("%s", args[1]);
            }
            //  printf("ka\n");
        }
        //printf("\n");
        // init_shell();
        free(newpath);
    }
    else if (strcmp(args[0], "echo") == 0)
    {
        for (int i = 1; i <= no_of_args; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
        // init_shell();
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[1024];
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
        // printf("\n");
        // init_shell();
    }
    else if (strcmp(args[0], "ls") == 0)
    {

        ls_implement(args, no_of_args);
        // printf("\n");
        // init_shell();
    }
    else if (strcmp(args[0], "pinfo") == 0)
    {
        pinfo(args, no_of_args);
        //printf("\n");
        //init_shell();
    }
    else if (strcmp(args[0], "history") == 0)
    {
        showhistory();
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "nightswatch") == 0)
    {
        nightswatch(args, no_of_args);
    }
    else
    {

        // printf("%ld\n", strlen(args[0]));
        if (strlen(args[0]) != 0)
        {
            system_commands(args, no_of_args);
        }
        // init_shell();
    }
}
