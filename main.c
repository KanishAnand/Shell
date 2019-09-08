#include "main.h"
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
#include "pinfo.h"
#include "wait_input.h"

#define clear() printf("\033[H\033[J")
// Global Variables
char home_dir[1024];
int background_pids[200];
int stopped_pids[200];
int no_of_backgroundprocess = 0;
char **background_process;

int main() {
    clear();
    background_process = (char **)malloc(2000 * sizeof(char **));
    // background_name = (char **)malloc(1000 * sizeof(char **));
    getcwd(home_dir, sizeof(home_dir));
    init_shell();

    while (1) {
        wait_input();
        // char st[1000] = "/proc/";
        int pid, pid_status;
        while ((pid = waitpid(-1, &pid_status, WNOHANG | WUNTRACED)) > 0) {
            if (WIFEXITED(pid_status)) {
                for (int i = 0; i < no_of_backgroundprocess; i++) {
                    if (background_pids[i] == pid) {
                        background_pids[i] = 0;
                        printf(
                            "Background process (%s) of 'PID' : %d exited "
                            "successfully \n",
                            background_process[i], pid);
                    }
                }
            } else if (WIFSIGNALED(pid_status)) {
                for (int i = 0; i < no_of_backgroundprocess; i++) {
                    if (background_pids[i] == pid) {
                        background_pids[i] = 0;
                        printf(
                            "Background process (%s) of 'PID' : %d exited "
                            "successfully \n",
                            background_process[i], pid);
                    }
                }
            }
            //     for (int i = 0; i < count; i++) {
            //         if (bg1[i] == pid) {
            //             bg1[i] = -1;
            //             // running_proc_size--;
            //             // char *t3;
            //             // t3 = get_bgname(pid);
            //             fprintf(stderr, "%s with pid %d exited normally \n",
            //                     names[i], pid);
            //             // free(running[i].pname);
            //             break;
            //         }
            //     }
            // } else if (WIFSIGNALED(pid_status)) {
            //     for (int i = 0; i < count; i++) {
            //         if (bg1[i] == pid) {
            //             bg1[i] = -1;
            //             // running_proc_size--;
            //             fprintf(stderr, "process with pid %d terminated \n",
            //                     pid);
            //             // free(running[i].pname);
            //             break;
            //         }
            //     }
            // }
        }
        // for (int i = 0; i < no_of_backgroundprocess; i++) {
        //     if (background_pids[i] != 0) {
        //         //	printf("%s\n", background_name[i]);
        //         int no = background_pids[i];
        //         char *val = (char *)malloc(sizeof(char) * 100);
        //         char *cat = (char *)malloc(sizeof(char) * 100);
        //         int cnt = 0;
        //         while (no > 0) {
        //             val[cnt] = no % 10 + '0';
        //             no /= 10;
        //             ++cnt;
        //         }

        //         --cnt;
        //         int j = 0;
        //         while (j <= cnt) {
        //             cat[j] = val[cnt - j];
        //             ++j;
        //         }
        //         strcat(st, cat);
        //         strcat(st, "/stat");

        //         int fd = open(st, O_RDONLY);

        //         if (fd == -1) {
        //             printf(
        //                 "Background process (%s) of 'PID' : %s exited "
        //                 "successfully \n",
        //                 background_process[i], cat);
        //             background_pids[i] = 0;

        //         } else {
        //             char *buff = (char *)calloc(1000, sizeof(char));
        //             read(fd, buff, 1000);
        //             char **parts = (char **)malloc(100 * sizeof(char *));
        //             int k = 0;
        //             while (k < 10) {
        //                 parts[k] = strsep(&buff, " ");
        //                 ++k;
        //             }
        //             if (strcmp(parts[2], "Z") == 0) {
        //                 // printf("%d\n", i);
        //                 printf(
        //                     "Background process (%s) of 'PID' : %s exited "
        //                     "successfully \n",
        //                     background_process[i], cat);
        //                 background_pids[i] = 0;
        //             }
        //             close(fd);
        //         }
        //     }
        // }
        init_shell();
    }

    return 0;
}