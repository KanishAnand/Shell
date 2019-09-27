#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "init_shell.h"
#include "main.h"

void jobs() {
    for (int i = 0; i < no_of_backgroundprocess; i++) {
        if (background_pids[i] != 0) {
            char **parts = (char **)malloc(100 * sizeof(char *));
            char *file = (char *)malloc(sizeof(char) * 100);
            char st[1000] = "/proc/";
            char b[] = "/stat";
            char *snum = (char *)malloc(sizeof(char) * 200);
            sprintf(snum, "%d", stopped_pids[i]);
            strcat(file, st);
            strcat(file, snum);
            strcat(file, b);
            int fd = open(file, O_RDONLY);
            if (fd == -1) {
                background_pids[i] = 0;
                break;
            } else {
                char *buff = (char *)calloc(300, sizeof(char));
                read(fd, buff, 300);
                int j = 0;
                while (j < 50) {
                    parts[j] = strsep(&buff, " ");
                    ++j;
                }
                close(fd);
            }

            char *str;
            if (parts[2][0] == 'R') {
                str = "Running";
            } else if (parts[2][0] == 'T' || parts[2][0] == 't') {
                str = "Stopped";
            } else if (parts[2][0] == 'D' || parts[2][0] == 'S') {
                str = "Sleeping";
            } else if (parts[2][0] == 'Z') {
                str = "Defunct/Zombie";
            }

            printf("[%d] %s %s %s [%d]\n", i + 1, parts[2], str,
                   background_process[i], stopped_pids[i]);
            // } else {
            //     printf("[%d] Stopped %s [%d]\n", i + 1,
            //     background_process[i],
            //            stopped_pids[i]);
            // }
        }
    }
}

void kjob(char **args, int no_of_args) {
    if (no_of_args != 2) {
        printf("Provide correct number of arguments\n");
        return;
    }
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

void overkill() {
    for (int i = 0; i < no_of_backgroundprocess; i++) {
        if (background_pids[i] != 0) {
            if (kill(stopped_pids[i], 9) == -1) {
                perror("kill");
            }
        }
    }
}

void fg(char **args, int no_of_args) {
    pid_t shellpid = getpid();
    int l = strlen(args[1]), no1 = 0, p = 1;
    for (int i = 0; i < l; i++) {
        no1 += (args[1][l - 1 - i] - '0') * p;
        p *= 10;
    }

    if (no1 > no_of_backgroundprocess) {
        printf("No such job number exists\n");
        return;
    }

    pid_t pidnumber;
    pidnumber = stopped_pids[no1 - 1];
    // printf("%d\n", pidnumber);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, getpgid(pidnumber));
    kill(pidnumber, SIGCONT);
    // signal(SIGTTOU, SIG_DFL);
    waitpid(pidnumber, NULL, WUNTRACED);
    // signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, shellpid);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}

void bg(char **args, int no_of_args) {
    int l = strlen(args[1]), no1 = 0, p = 1;
    for (int i = 0; i < l; i++) {
        no1 += (args[1][l - 1 - i] - '0') * p;
        p *= 10;
    }

    if (no1 > no_of_backgroundprocess) {
        printf("No such job number exists\n");
        return;
    }
    kill(stopped_pids[no1 - 1], SIGTTIN);
    kill(stopped_pids[no1 - 1], SIGCONT);
}

void cronjob(char **args, int no_of_args) {
    if (no_of_args != 6) {
        printf("Please provide correct number of arguments\n");
        return;
    }
    int l = strlen(args[4]), no1 = 0, p = 1;
    for (int i = 0; i < l; i++) {
        no1 += (args[4][l - 1 - i] - '0') * p;
        p *= 10;
    }
    int no2 = 0;
    p = 1, l = strlen(args[6]);
    for (int i = 0; i < l; i++) {
        no2 += (args[6][l - 1 - i] - '0') * p;
        p *= 10;
    }

    int number = no2 / no1;
    // char **pt = (char **)malloc(1000);
    // pt[0] = args[2];
    // pt[1] = 0;
    pid_t pid = fork();

    if (pid == 0) {
        setpgid(0, 0);
        close(STDERR_FILENO);
        sleep(no1);
        int original = number;
        while (number--) {
            // sleep(no1);
            // printf("%d\n", number);
            char **pt = (char **)malloc(1000);
            pt[0] = args[2];
            pt[1] = 0;
            // fflush(stdout);
            pid_t p = fork();
            if (p == -1) {
                perror("fork");
            }
            if (p == 0) {
                if (execvp(args[2], pt) == -1) {
                    perror("exec");
                }
                exit(0);
            }
            if (number != original - 1) {
                sleep(no1);
            }

            // fflush(stdout);
            // printf("sdf\n");
            // init_shell();
            // fflush(stdout);
        }
    }
}