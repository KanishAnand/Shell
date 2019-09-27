#include "wait_input.h"
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
#include "redirection.h"
#include "system_commands.h"

void wait_input() {
    int length = 500;
    char *buff = (char *)malloc(length * sizeof(char));
    processpid = 0;
    fgets(buff, length, stdin);

    char **token = (char **)malloc(40 * sizeof(char *));
    token[0] = strtok(buff, ";");

    int no_of_commands = 0;
    int len = strlen(token[0]);
    while (token[0][len - 1] == '\n' || token[0][len - 1] == '\t' ||
           token[0][len - 1] == ' ') {
        token[0][len - 1] = '\0';
        --len;
    }
    int p = 0;
    while (token[0][p] == '\n' || token[0][p] == '\t' || token[0][p] == ' ') {
        p++;
    }

    char *temp = "";
    temp = (char *)malloc(sizeof(char) * 100);
    int j = 0;

    for (int i = p; i < (int)strlen(token[0]); i++) {
        temp[j] = token[0][i];
        j++;
    }
    token[0] = temp;
    len = strlen(token[0]);

    while (len != 0 && token[no_of_commands] != NULL) {
        ++no_of_commands;
        token[no_of_commands] = strtok(NULL, ";");

        if (token[no_of_commands] == NULL) {
            break;
        }
        len = strlen(token[no_of_commands]);

        while (token[no_of_commands][len - 1] == '\n' ||
               token[no_of_commands][len - 1] == '\t' ||
               token[no_of_commands][len - 1] == ' ') {
            token[no_of_commands][len - 1] = '\0';
            --len;
        }

        p = 0;
        while (token[no_of_commands][p] == '\n' ||
               token[no_of_commands][p] == '\t' ||
               token[no_of_commands][p] == ' ') {
            p++;
        }

        temp = "";
        temp = (char *)malloc(sizeof(char) * 30);
        int j = 0;

        for (int i = p; i < (int)strlen(token[no_of_commands]); i++) {
            temp[j] = token[no_of_commands][i];
            j++;
        }
        token[no_of_commands] = temp;
        len = strlen(token[no_of_commands]);
    }

    int n = 0;

    while (n < no_of_commands) {
        int up_key = 0, count = 0;
        if (buff[0] == 27) {
            int l = strlen(token[n]);
            int f = 1;
            if (l % 3 == 0) {
                for (int i = 0; i < l; i += 3) {
                    if (token[n][i] != 27) {
                        f = 0;
                        break;
                    }
                }
            } else {
                f = 0;
            }
            if (f == 1) {
                up_key = 1;
                count += l / 3;
            }
        }

        if (up_key == 1) {
            char *st = (char *)malloc(1000);
            st[0] = 0;
            strcat(st, home_dir);
            strcat(st, "/");
            strcat(st, "his.txt");

            int fd = open(st, O_RDONLY);

            char *buff = (char *)malloc(4000);
            read(fd, buff, 1000);

            char **ptr = (char **)calloc(4000, 1);
            ptr[0] = strtok(buff, "\n");
            int j = 0;

            while (ptr[j] != NULL) {
                ++j;
                ptr[j] = strtok(NULL, "\n");
            }

            ptr[j] = 0;
            strcpy(token[n], ptr[j - count]);
            init_shell();
            printf("%s\n", token[n]);
            close(fd);
        }

        int flag = 0;
        for (int i = 0; i < (int)strlen(token[n]); i++) {
            if (token[n][i] == '|') {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            for (int i = 0; i < (int)strlen(token[n]); i++) {
                if (token[n][i] == '>' || token[n][i] == '<') {
                    flag = 2;
                    break;
                }
            }
        }
        char *dp = (char *)malloc(length * sizeof(char));
        strcpy(dp, token[n]);

        if (flag == 1) {
            redirection(token[n]);
        } else if (flag == 2) {
            ioredirect(token[n]);
        } else {
            char **parts = (char **)malloc(40);

            parts[0] = strtok(token[n], " \t");
            int no_of_args = 0;

            while (parts[no_of_args] != NULL) {
                ++no_of_args;
                parts[no_of_args] = strtok(NULL, " \t");
            }

            --no_of_args;

            // this is done because fgets puts a new line character at end
            // of read line so when we pass it to exectue command as arg
            // then chdir(arg) was not working. strlen(parts[i]);

            // this parts[i+1] = 0 is added because arg which is to be
            // passed to execvp should be ended with 0 so that it knows that
            // it ends otherwise we get error of no such file or directory
            // https://stackoverflow.com/questions/33813944/no-such-file-or-directory-when-using-execv
            parts[no_of_args + 1] = 0;
            execute_command(parts, no_of_args);
        }
        ++n;
        // char *dp = (char *)malloc(length * sizeof(char));
        // dp[strlen(dp) - 1] = '\0';
        // if (up_key != 1) {
        history(dp);
        // }
    }
}
