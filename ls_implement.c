#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"
#include "main.h"

void ls_implement(char **args, int no_of_args)
{
    DIR *p;
    struct dirent *d;
    if (no_of_args == 0 || strlen(args[1]) == 0)
    {
        //printf("%d\n", no_of_args);
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        args[1] = cwd;
    }

    char *newpath = (char *)malloc(sizeof(char) * 300);
    strcpy(newpath, home_dir);

    int l = strlen(home_dir);
    l--;
    if (args[1][0] == '~')
    {
        for (int i = 1; i < (int)strlen(args[1]); i++)
        {
            newpath[l + i] = args[1][i];
        }

        p = opendir(newpath);
        if (p == NULL)
        {
            perror("ls");
        }
        else
        {
            while ((d = readdir(p)))
            {
                char *nm = d->d_name;
                char s = nm[0];
                if (s != '.')
                {
                    printf("%s ", d->d_name);
                    printf("\n");
                }
            }
        }
    }

    else if (args[1][0] == '-')
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        p = opendir(cwd);

        if (strcmp(args[1], "-l") == 0)
        {
            long int sum = 0, c;
            while ((d = readdir(p)))
            {

                char *nm = d->d_name;
                char s = nm[0];
                if (s != '.')
                {
                    c = printFileProperties(nm);
                    sum += c / 2;
                    printf("%s\n", d->d_name);
                }
            }
            printf("total : %ld\n", sum);
        }
        else if (strcmp(args[1], "-a") == 0)
        {
            while ((d = readdir(p)))
            {
                // char *nm = d->d_name;
                // char s = nm[0];
                printf("%s ", d->d_name);
                printf("\n");
            }
        }
        else if (strcmp(args[1], "-la") == 0 || strcmp(args[1], "-al") == 0)
        {
            long int sum = 0, c;
            while ((d = readdir(p)))
            {
                char *nm = d->d_name;
                // char s = nm[0];
                c = printFileProperties(nm);
                sum += c / 2;
                printf("%s\n", d->d_name);
            }
            printf("total : %ld\n", sum);
        }
    }
    else
    {
        p = opendir(args[1]);
        if (p == NULL)
        {
            perror("ls");
        }
        else
        {
            while ((d = readdir(p)))
            {
                char *nm = d->d_name;
                char s = nm[0];
                if (s != '.')
                {
                    printf("%s ", d->d_name);
                    printf("\n");
                }
            }
        }
    }
}

long int printFileProperties(char *name)
{
    struct stat buff;
    lstat(name, &buff);
    struct tm *dt;
    printf((S_ISDIR(buff.st_mode)) ? "d" : "");
    printf((S_ISREG(buff.st_mode)) ? "-" : "");
    printf((S_ISLNK(buff.st_mode)) ? "l" : "");
    printf((buff.st_mode & S_IRUSR) ? "r" : "-");
    printf((buff.st_mode & S_IWUSR) ? "w" : "-");
    printf((buff.st_mode & S_IXUSR) ? "x" : "-");
    printf((buff.st_mode & S_IRGRP) ? "r" : "-");
    printf((buff.st_mode & S_IWGRP) ? "w" : "-");
    printf((buff.st_mode & S_IXGRP) ? "x" : "-");
    printf((buff.st_mode & S_IROTH) ? "r" : "-");
    printf((buff.st_mode & S_IWOTH) ? "w" : "-");
    printf((buff.st_mode & S_IXOTH) ? "x" : "-");
    printf("\t");
    printf("%ld\t", buff.st_nlink);
    printf("%s\t", getpwuid(buff.st_uid)->pw_name);
    printf("%s\t", getgrgid(buff.st_gid)->gr_name);
    printf("%ld\t", buff.st_size);
    char buffer[80];
    time_t timeStamp = buff.st_atime;
    dt = localtime(&timeStamp);
    strftime(buffer, 80, " %h %e\t", dt);
    printf("%s\t", buffer);
    char tm[20];
    strftime(tm, 20, "%T", localtime(&(buff.st_mtime)));
    printf("%s\t", tm);
    return buff.st_blocks;
}
