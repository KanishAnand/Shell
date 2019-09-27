#include "main.h"
#include <dirent.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include "execute_command.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "pinfo.h"
#include "signal_handling.h"
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
    signal(SIGTSTP, sigintHandlerZ);
    signal(SIGINT, sigintHandlerC);
    getcwd(home_dir, sizeof(home_dir));
    init_shell();

    while (1) {
        wait_input();
        int pid, pid_status;
        signal(SIGINT, sigintHandlerC);
        // signal(SIGTSTP, sigintHandlerZ);
        while ((pid = waitpid(-1, &pid_status, WNOHANG | WUNTRACED)) > 0) {
            if (WIFEXITED(pid_status)) {
                for (int i = 0; i < no_of_backgroundprocess; i++) {
                    if (background_pids[i] == pid) {
                        background_pids[i] = 0;
                        printf(
                            "Background process (%s) of 'PID' : %d exited "
                            "successfully \n",
                            background_process[i], pid);
                        break;
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
                        break;
                    }
                }
            }
        }
        init_shell();
    }

    return 0;
}