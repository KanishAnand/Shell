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

void system_commands(char **args, int no_of_args)
{
    FILE *fp = NULL;
    int status;
    pid_t pid = fork();
    pid_t sid = 0;
    if (pid == 0)
    {

        // int x = open("/dev/null", O_RDWR);
        // dup(x);
        setpgid(0, 0);
        umask(0);
        sid = setsid();
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        //close(STDERR_FILENO);
        // fp = fopen("mydaemonfile.txt", "w+");
        // int i = 0;
        // while (i < 5)
        // {
        //     sleep(1);
        //     //fprintf(fp, "%d", i);
        //     i++;
        // }
        // fclose(fp);
        // char cwd[1024];
        // printf("%s\n", getcwd(cwd, sizeof(cwd)));
        // printf("\n");
        // init_shell();
        if (execvp(args[0], args) == -1)
        {
            perror("exec");
        }
        printf("process completed");
    }
    if (pid > 0)
    {
        // int status;
        // (void)waitpid(pid, &status, 0);
        //waitpid(-1, &status, WNOHANG);
        printf("MM\n");
        // init_shell();
        // wait_input();
        // wait(NULL);
        // if (wait(0) == -1)
        // {
        //     perror("wait");
        // }
    }
}