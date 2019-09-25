#include "ioredirection.h"
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
#include "ls_implement.h"
#include "main.h"
#include "nightswatch.h"
#include "pinfo.h"
#include "system_commands.h"
#include "wait_input.h"

void ioredirect(char *args) {
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
    char **tkn = (char **)malloc(4000);
    cmnd[0] = strtok(argscp1, ">");
    cmnd[0] = strtok(cmnd[0], "<");
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
        wait(NULL);
    }
}

// void outputredirect(char *args) {
//     char **parts = (char **)malloc(4000);
//     parts[0] = strtok(args, ">");
//     int i = 0;
//     while (parts[i] != NULL) {
//         ++i;
//         parts[i] = strtok(NULL, ">");
//     }

//     parts[i] = 0;
//     char **cmnd = (char **)malloc(1000);
//     cmnd[0] = strtok(parts[0], " \t");
//     i = 0;
//     while (cmnd[i] != NULL) {
//         ++i;
//         cmnd[i] = strtok(NULL, ">");
//     }

//     parts[1] = strtok(parts[1], " \t");
//     cmnd[i] = 0;
//     pid_t p = fork();
//     if (p == 0) {
//         // printf("%s %d\n", parts[1], strlen(parts[1]));
//         int fd = open(parts[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         dup2(fd, STDOUT_FILENO);
//         execvp(cmnd[0], cmnd);
//     } else {
//         wait(NULL);
//     }
// }

// void inputredirect(char *args) {
//     char **parts = (char **)malloc(4000);
//     parts[0] = strtok(args, "<");
//     int i = 0;
//     while (parts[i] != NULL) {
//         ++i;
//         parts[i] = strtok(NULL, "<");
//     }

//     parts[i] = 0;
//     char **cmnd = (char **)malloc(1000);
//     cmnd[0] = strtok(parts[0], " \t");
//     i = 0;
//     while (cmnd[i] != NULL) {
//         ++i;
//         cmnd[i] = strtok(NULL, "<");
//     }

//     parts[1] = strtok(parts[1], " \t");
//     cmnd[i] = 0;
//     pid_t p = fork();
//     if (p == 0) {
//         int fd = open(parts[1], O_RDONLY);
//         if (fd != -1) {
//             dup2(fd, STDIN_FILENO);
//             execvp(cmnd[0], cmnd);
//         } else {
//             perror(parts[1]);
//         }
//     } else {
//         wait(NULL);
//     }
// }