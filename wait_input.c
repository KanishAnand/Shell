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
#include "history.h"

void wait_input()
{
    int length = 500;
    char *buff = (char *)malloc(length * sizeof(char));
    fgets(buff, length, stdin);
    char *dp = (char *)malloc(length * sizeof(char));
    strcpy(dp, buff);

    // printf("k\n");
    // printf("%d\n", &dup);

    // int cnt = 0;
    char **token = (char **)malloc(40 * sizeof(char *));
    // for (int i = 0; i < 25; i++)
    // {
    //     token[i] = (char *)malloc(400 * sizeof(char));
    // }
    token[0] = strtok(buff, ";");

    int no_of_commands = 0;
    int len = strlen(token[0]);
    while (token[0][len - 1] == '\n' || token[0][len - 1] == '\t' || token[0][len - 1] == ' ')
    {
        token[0][len - 1] = '\0';
        --len;
    }
    int p = 0;
    while (token[0][p] == '\n' || token[0][p] == '\t' || token[0][p] == ' ')
    {
        p++;
    }

    char *temp = "";
    temp = (char *)malloc(sizeof(char) * 30);
    int j = 0;

    for (int i = p; i < strlen(token[0]); i++)
    {
        temp[j] = token[0][i];
        j++;
    }
    token[0] = temp;
    len = strlen(token[0]);

    while (len != 0 && token[no_of_commands] != NULL)
    {
        ++no_of_commands;
        token[no_of_commands] = strtok(NULL, ";");

        if (token[no_of_commands] == NULL)
        {
            break;
        }
        len = strlen(token[no_of_commands]);

        while (token[no_of_commands][len - 1] == '\n' || token[no_of_commands][len - 1] == '\t' || token[no_of_commands][len - 1] == ' ')
        {
            token[no_of_commands][len - 1] = '\0';
            --len;
        }

        p = 0;
        while (token[no_of_commands][p] == '\n' || token[no_of_commands][p] == '\t' || token[no_of_commands][p] == ' ')
        {
            p++;
        }

        temp = "";
        temp = (char *)malloc(sizeof(char) * 30);
        int j = 0;

        for (int i = p; i < strlen(token[no_of_commands]); i++)
        {
            temp[j] = token[no_of_commands][i];
            j++;
        }
        token[no_of_commands] = temp;
        len = strlen(token[no_of_commands]);
    }

    int n = 0;
    while (n < no_of_commands)
    {
        // int le = strlen(token[n]);
        // while (token[n][0] == '\n' || token[n][0] == '\t')
        // {
        //     token[n][0] = '\0';
        // }
        // while (token[n][le - 1] == '\n' || token[n][le - 1] == '\t')
        // // if (token[n][le - 1] == '\n')
        // {
        //     token[n][le - 1] = '\0';
        //     le--;
        // }

        char **parts = (char **)malloc(40 * sizeof(char *));
        // for (int i = 0; i < 5; i++)
        //     parts[i] = (char *)malloc(400 * sizeof(char));

        parts[0] = strtok(token[n], " ");
        int i = 0;

        while (parts[i] != NULL)
        {
            ++i;
            parts[i] = strtok(NULL, " ");
        }

        --i;

        //this is done because fgets puts a new line character at end of read line so when we pass it to exectue command as arg then chdir(arg) was not working.
        // strlen(parts[i]);

        //this parts[i+1] = 0 is added because arg which is to be passed to execvp should be ended with 0 so that it knows that it ends otherwise we get error of
        //no such file or directory https://stackoverflow.com/questions/33813944/no-such-file-or-directory-when-using-execv
        parts[i + 1] = 0;
        execute_command(parts, i);
        ++n;
        // printf("kan\n");
        dp[strlen(dp) - 1] = '\0';
        history(dp);
        free(dp);
        free(token);
        free(parts);
        free(temp);
        free(buff);
        //printf("%s\n", token);
    }
}
