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

void outputredirect(char *args) {
    char **parts = (char **)malloc(sizeof(4000));
    parts[0] = strtok(args, ">");
    int i = 0;
    while (parts[i] != NULL) {
        ++i;
        parts[i] = strtok(NULL, ">");
    }

    parts[i] = 0;
    char **cmnd = (char **)malloc(sizeof(1000));
    cmnd[0] = strtok(parts[0], " \t");
    i = 0;
    while (cmnd[i] != NULL) {
        ++i;
        cmnd[i] = strtok(NULL, ">");
    }

    cmnd[i] = 0;
    pid_t p = fork();
    if (p == 0) {
        // printf("%s %d\n", parts[1], strlen(parts[1]));
        int fd = open(parts[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        execvp(cmnd[0], cmnd);
    } else {
        wait(NULL);
    }
}