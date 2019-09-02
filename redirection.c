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

void redirection(char *args)
{
    char **parts = (char **)malloc(400);
    parts[0] = strtok(args, "|");
    int no_of_args = 0;

    while (parts[no_of_args] != NULL)
    {
        ++no_of_args;
        parts[no_of_args] = strtok(NULL, "|");
    }

    --no_of_args;
    parts[no_of_args + 1] = 0;

    char **cmnd = (char **)malloc(400);
    int i = 0;
    cmnd[0] = strtok(parts[0], " \t");
    while (cmnd[i] != NULL)
    {
        ++i;
        cmnd[i] = strtok(NULL, " \t");
    }
    cmnd[i] = 0;
    char **cmnd2 = (char **)malloc(400);
    i = 0;
    cmnd2[0] = strtok(parts[1], " \t");
    while (cmnd2[i] != NULL)
    {
        ++i;
        cmnd2[i] = strtok(NULL, " \t");
    }
    cmnd2[i] = 0;

    // main piping working code

    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0)
    {
        printf("\nCould not fork");
        return;
    }
    if (p1 == 0)
    {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);

        // here we need to duplicate it because execvp used below reads from stdin and outputs to stdout but we want to read and write from pipe ends .
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(cmnd[0], cmnd) < 0)
        {
            printf("\nCould not execute command 1..");
        }
        exit(0);
    }
    else
    {
        // Parent executing
        p2 = fork();

        if (p2 < 0)
        {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(cmnd2[0], cmnd2) < 0)
            {
                printf("\nCould not execute command 2..");
            }
            exit(0);
        }
        else
        {
            // parent executing, waiting for two children
            // wait(NULL);
            // wait(NULL);
            exit(0);
        }
    }
}