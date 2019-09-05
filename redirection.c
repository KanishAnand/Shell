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
#include "execute_command.h"
#include "history.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "main.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "system_commands.h"
#include "wait_input.h"

void redirection(char *args) {
    char **parts = (char **)malloc(400);
    parts[0] = strtok(args, "|");
    int no_of_pipes = 0;

    while (parts[no_of_pipes] != NULL) {
        ++no_of_pipes;
        parts[no_of_pipes] = strtok(NULL, "|");
    }

    --no_of_pipes;
    parts[no_of_pipes + 1] = 0;

    // main piping working code

    int pipefd[2];
    int pipefd2[2];
    // here we need to duplicate it because execvp used below reads from
    // stdin and outputs to stdout but we want to read and write from pipe
    //     ends . dup2(pipefd[1], STDOUT_FILENO); for (int j = 0; j < 2 *
    int cnt = 0, p = 0;

    while (cnt <= no_of_pipes) {
        if (cnt % 2 == 0) {
            if (pipe(pipefd) < 0) {
                printf("\nPipe could not be initialized");
                return;
            }
        } else {
            if (pipe(pipefd2) < 0) {
                printf("\nPipe could not be initialized");
                return;
            }
        }

        char **cmnd2 = (char **)malloc(400);
        cmnd2[0] = strtok(parts[p], " \t");
        int j = 0;

        ++p;
        while (cmnd2[j] != NULL) {
            ++j;
            cmnd2[j] = strtok(NULL, " \t");
        }
        cmnd2[j] = 0;

        // Parent executing
        pid_t p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            if (cnt % 2 == 0) {
                if (cnt != 0) {
                    dup2(pipefd2[0], STDIN_FILENO);
                }
                if (cnt != no_of_pipes) {
                    dup2(pipefd[1], STDOUT_FILENO);
                }
            } else {
                dup2(pipefd[0], STDIN_FILENO);
                if (cnt != no_of_pipes) {
                    dup2(pipefd2[1], STDOUT_FILENO);
                }
            }

            if (execvp(cmnd2[0], cmnd2) < 0) {
                printf("\nCould not execute command 2..");
            }
            exit(0);
        } else {
            if (cnt % 2 == 0) {
                if (cnt != 0) {
                    close(pipefd2[0]);
                }
                if (cnt != no_of_pipes) {
                    close(pipefd[1]);
                }
            } else {
                close(pipefd[0]);
                if (cnt != no_of_pipes) {
                    close(pipefd2[1]);
                }
            }
            cnt += 1;
            wait(NULL);
        }
    }
}