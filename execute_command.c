#include "execute_command.h"
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include "history.h"
#include "init_shell.h"
#include "jobs.h"
#include "ls_implement.h"
#include "main.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "system_commands.h"
#include "wait_input.h"

void execute_command(char **args, int no_of_args) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || strlen(args[1]) == 0 ||
            strcmp(args[1], "~") == 0) {
            args[1] = home_dir;
        }

        char *newpath = (char *)malloc(3000);

        if (args[1][0] == '~') {
            strcat(newpath, home_dir);
            strcat(newpath, args[1] + 1);
        } else
            strcpy(newpath, args[1]);

        if (chdir(newpath) == -1) {
            perror("cd");
        }
    } else if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; i <= no_of_args; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
        // init_shell();
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
    } else if (strcmp(args[0], "ls") == 0) {
        ls_implement(args, no_of_args);
    } else if (strcmp(args[0], "pinfo") == 0) {
        pinfo(args, no_of_args);
    } else if (strcmp(args[0], "history") == 0) {
        showhistory();
    } else if (strcmp(args[0], "quit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "nightswatch") == 0) {
        pid_t p = fork();
        if (p == 0) {
            nightswatch(args, no_of_args);
        } else {
            processpid = p;
        }
    } else if (strcmp(args[0], "setenv") == 0) {
        char *st = (char *)malloc(2000);
        // strcat(st, "\"");
        strcat(st, args[1]);
        strcat(st, "=");
        if (args[2] == NULL) {
            args[2] = " ";
        }
        strcat(st, args[2]);
        // strcat(st, "\"");
        if (no_of_args != 2 && no_of_args != 1) {
            printf("Provide correct number of arguments\n");
        } else {
            if (putenv(st) == -1) {
                perror("setenv");
            } else {
                char *pathvar;
                pathvar = getenv(args[1]);
                printf("The current path is: %s\n", pathvar);
            }
        }
    } else if (strcmp(args[0], "unsetenv") == 0) {
        if (no_of_args != 1) {
            printf("Provide correct number of arguments\n");
        } else {
            char *pathvar;
            pathvar = getenv(args[1]);
            printf("The current path is: %s\n", pathvar);
            unsetenv(args[1]);
            // char *pathvar;
            pathvar = getenv(args[1]);
            printf("The current path is: %s\n", pathvar);
        }
    } else if (strcmp(args[0], "jobs") == 0) {
        jobs();
    } else if (strcmp(args[0], "kjob") == 0) {
        kjob(args, no_of_args);
    } else if (strcmp(args[0], "fg") == 0) {
        fg(args, no_of_args);
    } else if (strcmp(args[0], "bg") == 0) {
        bg(args, no_of_args);
    } else if (strcmp(args[0], "overkill") == 0) {
        overkill();
    } else if (strcmp(args[0], "cronjob") == 0) {
        cronjob(args, no_of_args);
    } else {
        // printf("%ld\n", strlen(args[0]));
        if (strlen(args[0]) != 0) {
            system_commands(args, no_of_args);
        }
        // init_shell();
    }
}
