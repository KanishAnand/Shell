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
#include "main.h"
int cnt = 0;

void push(char *in, char **qu) {
    qu[cnt] = in;
    cnt++;
}

void history(char *rd) {
    cnt = 0;

    char **qu = (char **)malloc(sizeof(char *) * 500);
    char st[100] = " ";
    strcpy(st, home_dir);
    strcat(st, "/");
    strcat(st, "his.txt");

    int fd = open(st, O_RDWR | O_CREAT, 0677);
    char *buff = (char *)(calloc(1000, 1));
    read(fd, buff, 1000);
    char **parts = (char **)malloc(50 * sizeof(char *));
    parts[0] = strtok(buff, "\n");

    if (parts[0] != NULL) {
        push(parts[0], qu);
    }
    int i = 0;
    while (parts[i] != NULL) {
        ++i;
        parts[i] = strtok(NULL, "\n");
        if (parts[i] != NULL) {
            push(parts[i], qu);
        }
    }
    --i;

    if (parts[0] != NULL) {
        if (strcmp(parts[i], rd) != 0) {
            push(rd, qu);
        }
    } else {
        push(rd, qu);
    }

    close(fd);
    fd = open(st, O_WRONLY | O_TRUNC, 0677);
    int p = 0;
    if (cnt - 20 > 0) {
        p = cnt - 20;
    }
    for (int i = p; i < cnt; i++) {
        write(fd, qu[i], strlen(qu[i]));
        if (i < cnt - 1) {
            write(fd, "\n", 1);
        }
    }
    close(fd);
}

void showhistory() {
    char **qu = (char **)malloc(500);

    char *st = (char *)malloc(1000);
    strcpy(st, home_dir);
    strcat(st, "/");
    strcat(st, "his.txt");

    int fd = open(st, O_RDONLY);

    char *buff = (char *)malloc(4000);
    read(fd, buff, 1000);

    char *ptr = strtok(buff, "\n");
    cnt = 0;
    if (ptr) {
        qu[cnt++] = ptr;
        while ((ptr = strtok(NULL, "\n"))) {
            qu[cnt++] = ptr;
        }
    }

    close(fd);
    int p = 0;
    if (cnt > 10) {
        p = cnt - 10;
    }

    for (int i = p; i < cnt; i++) {
        printf("%s\n", qu[i]);
    }
}
