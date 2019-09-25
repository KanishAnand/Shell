#include <dirent.h>
#include <fcntl.h>
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
#include "ioredirection.h"
#include "ls_implement.h"
#include "main.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "system_commands.h"
#include "wait_input.h"

void ioredirect1(char *args) {
    int output_right = -1, input_right = -1, double_redirect = 0;
    char *argscp = (char *)malloc(4000);
    char *argscp1 = (char *)malloc(4000);
    char *output_arg = (char *)malloc(1000);
    char *input_arg = (char *)malloc(1000);
    strcpy(argscp, args);
    strcpy(argscp1, args);

    int l = strlen(args);
    for (int i = l - 1; i >= 0; i--) {
        if (args[i] == '<') {
            input_right = i;
            break;
        }
    }
    for (int i = l - 1; i >= 0; i--) {
        if (args[i] == '>') {
            output_right = i;
            break;
        }
    }

    if (output_right != -1) {
        if (output_right > 0 && args[output_right - 1] == '>') {
            double_redirect = 1;
        }
    }

    char **cmnd = (char **)malloc(4000);
    // char *cmnds = (char *)malloc(1000);
    char **tkn = (char **)malloc(4000);
    cmnd[0] = strtok(argscp1, ">");
    cmnd[0] = strtok(cmnd[0], "<");
    // cmnds = strtok(cmnd[0], " \t");
    int z = 0;
    tkn[0] = strtok(cmnd[0], " \t");
    while (tkn[z] != NULL) {
        ++z;
        tkn[z] = strtok(NULL, " \t");
    }

    if (output_right != -1) {
        char **parts = (char **)malloc(4000);
        parts[0] = strtok(args, ">");
        int i = 0;
        while (parts[i] != NULL) {
            ++i;
            parts[i] = strtok(NULL, ">");
        }
        parts[i] = 0;
        output_arg = parts[i - 1];
        output_arg = strtok(output_arg, "<");
        output_arg = strtok(output_arg, " \t");
    }

    if (input_right != -1) {
        char **parts = (char **)malloc(4000);
        parts[0] = strtok(argscp, "<");
        int i = 0;
        while (parts[i] != NULL) {
            ++i;
            parts[i] = strtok(NULL, "<");
        }
        parts[i] = 0;
        input_arg = parts[i - 1];
        input_arg = strtok(input_arg, ">");
        input_arg = strtok(input_arg, " \t");
    }

    pid_t p = fork();
    if (p == 0) {
        if (input_right != -1) {
            int fd = open(input_arg, O_RDONLY);

            if (fd == -1) {
                perror(input_arg);
                exit(0);
            } else {
                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(0);
                }
            }
        }
        if (output_right != -1) {
            int fd1;
            if (double_redirect == 0) {
                fd1 = open(output_arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else {
                fd1 = open(output_arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            if (fd1 == -1) {
                perror(output_arg);
                exit(0);
            } else {
                if (dup2(fd1, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(0);
                }
            }
        }
        if (execvp(tkn[0], tkn) == -1) {
            perror(tkn[0]);
            exit(0);
        }
    } else {
        // wait(NULL);
        exit(0);
    }
}

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

        char *partscp = (char *)malloc(400);
        strcpy(partscp, parts[p]);
        char **cmnd2 = (char **)malloc(4000);
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

            int l = strlen(partscp), flag = 0;

            for (int i = 0; i < l; i++) {
                if (partscp[i] == '>' || partscp[i] == '<') {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                if (execvp(cmnd2[0], cmnd2) < 0) {
                    printf("\nCould not execute command 2..");
                }
                exit(0);
            } else {
                ioredirect1(partscp);
            }
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