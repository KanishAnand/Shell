#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
int cnt = 0;

void push(char in[], char **qu)
{
    //strcpy(qu[cnt], in);
    // printf("%d\n", cnt);
    // printf("l%sp\n", in);
    qu[cnt] = in;
    //printf("%s\n", qu[cnt]);
    cnt++;
}

void history(char *rd)
{
    //printf("kan\n");
    cnt = 0;
    // printf("%ld\n", strlen(rd));
    // char qu[30][200];
    // for (int i = 0; i < 30; i++)
    // {
    //     qu[i] = "df";
    // }

    char **qu = (char **)malloc(sizeof(char *) * 50);
    //printf("kan\n");
    // for (int i = 0; i < 50; i++)
    // {
    //     qu[i] = (char *)malloc(+100 * sizeof(char));
    // }
    // printf("kan3\n");
    // char *st = (char *)malloc(sizeof(char) * 1000);
    char st[] = " ";
    strcpy(st, home_dir);
    // strcat(st, home_dir);
    strcat(st, "/");
    strcat(st, "his.txt");

    int fd = open(st, O_RDWR | O_CREAT, 0677);
    // FILE *file = fopen(str, "r");
    // int fd = open()
    // char *buff = ;
    char line[256];
    size_t len = 0;
    char *buff = (char *)(calloc(1000, 1));
    // rewind(file);
    // fseek(file, 0, SEEK_SET);
    int x = read(fd, buff, 1000);
    // while (!feof(file))
    // {
    //     getc(buff, sizeof(buff), 1, file);
    //     printf("%s\n", buff);
    // }
    //char ch = getc(file);
    //printf("%c", ch);
    // printf("%s\n", fgets(line, sizeof(line), file));
    // while (fgets(line, sizeof(line), file))
    // {
    //     /* note that fgets don't strip the terminating \n, checking its
    //        presence would allow to handle lines longer that sizeof(line) */
    //     printf("%s", line);
    // }

    //printf("%d\n", getline(&buff, &len, file));
    // while (getline(&buff, &len, file) != -1)
    // {
    //     //     // pri
    //     //     printf("s\n");
    //     printf("%s\n", buff);
    //     //     //push(buff, qu);
    // }
    // printf("%dkan\n", fd);
    //printf("kan4\n");
    // printf("%s\n", buff);
    char **parts = (char **)malloc(50 * sizeof(char *));
    // for (int i = 0; i < 50; i++)
    // {
    //     parts[i] = (char *)malloc(300 * sizeof(char));
    // }
    //printf("kan1\n");
    //printf("kan\n");
    parts[0] = strtok(buff, "\n");
    // printf("ss\n");
    // printf("%s\n", parts[0]);
    if (parts[0] != NULL)
    {
        push(parts[0], qu);
    }
    // printf("tt\n");
    int i = 0;
    // printf("kan\n");
    while (parts[i] != NULL)
    {
        ++i;
        // printf("%d\n", i);
        parts[i] = strtok(NULL, "\n");
        if (parts[i] != NULL)
        {
            push(parts[i], qu);
        }
    }
    // //printf("kan\n");
    // //  printf("kan\n");
    // printf("%d\n", i);
    --i;
    // for (int j = 0; j < i; j++)
    // {
    //     printf("%s\n", parts[j]);
    // }
    // printf("d%s %s\n", parts[i], rd);
    // printf("%ld %ld", strlen(parts[i]), strlen(rd));

    if (strcmp(parts[i], rd) != 0)
    {
        push(rd, qu);
    }
    //fclose(file);
    close(fd);
    fd = open(st, O_WRONLY | O_TRUNC, 0677);
    // close(fd);
    // printf("%s\n", rd);
    // printf("%d\n", cnt);
    int p = 0;
    if (cnt - 20 > 0)
    {
        p = cnt - 20;
    }
    for (int i = p; i < cnt; i++)
    {
        write(fd, qu[i], strlen(qu[i]));
        if (i < cnt - 1)
        {
            write(fd, "\n", 1);
        }
    }
    // printf("%d\n", cnt);
    // for (int i = 0; i < 14; i++)
    // {
    //     printf("%s\n", qu[i]);
    // }
    //  write(fd, "kan", 3);
    // close(fd);
    // for (int i = 0; i < 49; i++)
    // {
    //     free(qu[i]);
    // }
    // for (int i = 0; i < 50; i++)
    // {
    //     free(parts[i]);
    // }
    // free(qu);
    // fclose(file);
    // free(buff);
    //free(parts);
    //free(rd);
    // free(st);
    close(fd);
}

void showhistory()
{
    cnt = 0;
    char **qu = (char **)malloc(sizeof(char *) * 50);
    char st[] = " ";
    strcpy(st, home_dir);
    strcat(st, "/");
    strcat(st, "his.txt");

    int fd = open(st, O_RDWR | O_CREAT, 0677);
    char line[256];
    size_t len = 0;
    char *buff = (char *)(calloc(1000, 1));
    int x = read(fd, buff, 1000);
    char **parts = (char **)malloc(50 * sizeof(char *));
    parts[0] = strtok(buff, "\n");
    if (parts[0] != NULL)
    {
        push(parts[0], qu);
    }
    int i = 0;
    while (parts[i] != NULL)
    {
        ++i;
        parts[i] = strtok(NULL, "\n");
        if (parts[i] != NULL)
        {
            push(parts[i], qu);
        }
    }
    --i;
    close(fd);
    int p = 0;
    if (cnt - 10 > 0)
    {
        p = cnt - 10;
    }
    for (int i = p; i < cnt; i++)
    {
        printf("%s\n", qu[i]);
    }
}
