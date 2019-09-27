#include "pinfo.h"
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
#include "init_shell.h"
#include "ls_implement.h"
#include "wait_input.h"

void pinfo(char **args, int no_of_args) {
    char *file = (char *)malloc(sizeof(char) * 100);
    char st[1000] = "/proc/";
    char b[] = "/stat";

    if (no_of_args == 0 || strlen(args[1]) == 0) {
        file = "/proc/self/stat";
    } else {
        strcat(file, st);
        strcat(file, args[1]);
        strcat(file, b);
    }
    // a[2] = 0;
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("pinfo");
    } else {
        char *buff = (char *)calloc(300, sizeof(char));
        read(fd, buff, 300);

        char **parts = (char **)malloc(100 * sizeof(char *));
        // for (int i = 0; i < 60; i++)
        // {
        //     parts[i] = (char *)malloc(500 * sizeof(char));
        // }

        int i = 0;
        while (i < 50) {
            parts[i] = strsep(&buff, " ");
            ++i;
        }
        char st[100] = "~/";
        for (int i = 1; i < (int)strlen(parts[1]) - 1; i++) {
            st[i + 1] = parts[1][i];
        }
        printf("PID -- %s\n", parts[0]);
        printf("Process Status -- %s\n", parts[2]);
        printf("memory -- %s\n", parts[22]);
        printf("Executable Path -- %s\n", st);
        close(fd);
    }
}