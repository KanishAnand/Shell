#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void jobs() {
    for (int i = 0; i < no_of_backgroundprocess; i++) {
        if (background_pids[i] != 0) {
            printf("[%d] Running %s [%d]\n", i + 1, background_process[i],
                   stopped_pids[i]);
        } else {
            printf("[%d] Stopped %s [%d]\n", i + 1, background_process[i],
                   stopped_pids[i]);
        }
    }
}

void kjob(char **args, int no_of_args) {
    int l = strlen(args[1]), no1 = 0, p = 1;
    for (int i = 0; i < l; i++) {
        no1 += (args[1][l - 1 - i] - '0') * p;
        p *= 10;
    }
    l = strlen(args[2]), p = 1;
    int no2 = 0;
    for (int i = 0; i < l; i++) {
        no2 += (args[2][l - 1 - i] - '0') * p;
        p *= 10;
    }
    if (no1 > no_of_backgroundprocess) {
        printf("No such background process exists\n");
    } else {
        if (kill(stopped_pids[no1 - 1], no2) == -1) {
            perror("kill");
        }
    }
}