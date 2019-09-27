#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "init_shell.h"
#include "main.h"
#include "nightswatch.h"
#include "system_commands.h"

// function to handle Ctrl-C
void sigintHandlerC(int sig_num) {
    pid_t process = processpid;
    if (process > 0) {
        kill(process, sig_num);
        // signal(SIGTSTP, sigintHandlerZ);
        printf("\n");
        // // init_shell();
        fflush(stdout);
    } else {
        printf("\n");
        init_shell();
        fflush(stdout);
    }
}

void sigintHandlerZ(int sig_num) {
    pid_t process = processpid;
    // kill(process, SIGTTIN);
    if (process > 0) {
        kill(process, sig_num);
        // signal(SIGTSTP, sigintHandlerZ);
        printf("\n");
        // init_shell();
        fflush(stdout);
        background_pids[no_of_backgroundprocess] = process;
        stopped_pids[no_of_backgroundprocess] = process;

        char *file = (char *)malloc(sizeof(char) * 100);
        char st[1000] = "/proc/";
        char b[] = "/stat";
        char *snum = (char *)malloc(sizeof(char) * 200);
        sprintf(snum, "%d", process);
        strcat(file, st);
        strcat(file, snum);
        strcat(file, b);
        int fd = open(file, O_RDONLY);
        if (fd == -1) {
            perror("Ctrl-Z");
        } else {
            char *buff = (char *)calloc(300, sizeof(char));
            read(fd, buff, 300);
            char **parts = (char **)malloc(100 * sizeof(char *));
            int i = 0;
            while (i < 50) {
                parts[i] = strsep(&buff, " ");
                ++i;
            }
            close(fd);
            background_process[no_of_backgroundprocess] = parts[1];
            no_of_backgroundprocess++;
        }

    } else {
        printf("\n");
        init_shell();
        fflush(stdout);
    }
}

void signal_handling() {
    char c = getchar();
    while (1) {
        if (c == 'q') {
            break;
        }
    }
}
