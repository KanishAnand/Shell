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
#include "main.h"
#include "wait_input.h"
pid_t processpid;
// as we have to use execvp so we need to make a child process bcz otherwise
// execvp replaces the current running process with a new process

void system_commands(char **args, int no_of_args) {
    int background = 0;
    if (strcmp(args[no_of_args], "&") == 0) {
        background = 1;
    }

    pid_t pid = fork();
    // processpid = 0;
    if (pid == 0) {
        if (background == 1) {
            // this changes the group pid of the process to 0
            setpgid(0, 0);
            // umask(0);
            // sid = setsid();
            // close the input and error printing of background process but not
            // output as we want the final result
            // close(STDIN_FILENO);
            // close(STDOUT_FILENO);
            close(STDERR_FILENO);
            args[no_of_args] = 0;
        }
        // args = strtok(args, "&");

        if (execvp(args[0], args) == -1) {
            perror("exec");
        }
        exit(0);
    }
    if (pid > 0) {
        // processpid = pid;
        if (background == 0) {
            processpid = pid;

            // this blocks parent process until all its children process not
            // gets finished.
            int status;
            waitpid(pid, &status, WUNTRACED);
        } else {
            // this prevents making of zombie process which  is very important
            // https: //www.geeksforgeeks.org/zombie-processes-prevention/
            // signal(SIGCHLD, SIG_IGN);
            // sighandler(SIGCHLD, put function here???);
            // signal(SIGCHLD, func);
            printf("%d\n", pid);
            background_pids[no_of_backgroundprocess] = pid;
            stopped_pids[no_of_backgroundprocess] = pid;
            background_process[no_of_backgroundprocess] = args[0];
            no_of_backgroundprocess++;
        }
    }
}
