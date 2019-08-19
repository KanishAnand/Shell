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

void wait_input()
{
    int length = 200;
    // int length_2d = 200000;
    char *buff = (char *)malloc(length * sizeof(char));
    fgets(buff, length, stdin);
    //char *part1,*part2;
    //char *parts = (char*)malloc(length_2d*sizeof(char));
    char **parts = (char **)malloc(40 * sizeof(char *));
    for (int i = 0; i < 5; i++)
        parts[i] = (char *)malloc(400 * sizeof(char));

    int i = 0;
    while (1)
    {
        parts[i] = strsep(&buff, " ");
        int l = strlen(parts[i]);
        if (parts[i][l - 1] == '\n')
        {
            break;
        }
        ++i;
    }

    //this is done because fgets puts a new line character at end of read line so when we pass it to exectue command as arg then chdir(arg) was not working.
    // part2[l-1] = '\0';
    // printf("%d\n",i);
    int l = strlen(parts[i]);
    parts[i][l - 1] = '\0';
    execute_command(parts, i);
}
