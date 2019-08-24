#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"
#include "main.h"

// as we have to use execvp so we need to make a child process bcz otherwise execvp replaces the current running process with a new process

void system_commands(char **args, int no_of_args)
{
    int background = 0;
    if (strcmp(args[no_of_args], "&") == 0)
    {
        background = 1;
    }

    pid_t pid = fork();
    if (pid == 0)
    {

        // int x = open("/dev/null", O_RDWR);
        // dup(x);

        if (background == 1)
        {
            // this changes the group pid of the process to 0
            setpgid(0, 0);
            //umask(0);
            //sid = setsid();
            // close the input and error printing of background process but not output as we want the final result
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }
        // fp = fopen("mydaemonfile.txt", "w+");
        // int i = 0;
        // while (i < 10)
        // {
        //     sleep(1);
        //     //fprintf(fp, "%d", i);
        //     i++;
        // }
        //fclose(fp);
        // char cwd[1024];
        // printf("%s\n", getcwd(cwd, sizeof(cwd)));
        // printf("\n");
        // init_shell();
        // printf("kF\n");
        //execvp(args[0], args);
        if (execvp(args[0], args) == -1)
        {
            perror("exec");
        }
        exit(0);
        //printf("kk\n");
        // if (background == 1)
        // {
        //     printf("Background Process Completed");
        // }
    }
    if (pid > 0)
    {

        if (background == 0)
        {
            //this blocks parent process until all its children process not gets finished.
            wait(NULL);
        }
        else
        {
            // this prevents making of zombie process which  is very important
            //https: //www.geeksforgeeks.org/zombie-processes-prevention/
            //signal(SIGCHLD, SIG_IGN);
            // sighandler(SIGCHLD, put function here???);
            // signal(SIGCHLD, func);
            printf("%d\n", pid);
            background_pids[no_of_backgroundprocess] = pid;
            // background_name[no_of_backgroundprocess] = args[0];
            // printf("%d\n", no_of_backgroundprocess);
            // printf("%s\n", background_name[no_of_backgroundprocess]);
            no_of_backgroundprocess++;
        }
        // printf("kk\n");
        // if (wait(0) == -1)
        // {
        //     perror("wait");
        // }
    }
}
