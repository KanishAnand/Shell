#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"
#include "pinfo.h"
#include "main.h"

#define clear() printf("\033[H\033[J")
//Global Variables
char home_dir[1024];
int background_pids[200];
int no_of_backgroundprocess = 0;

int main()
{
	clear();
	//background_name = (char **)malloc(1000 * sizeof(char **));
	getcwd(home_dir, sizeof(home_dir));
	init_shell();

	while (1)
	{
		wait_input();
		char st[1000] = "/proc/";
		for (int i = 0; i < no_of_backgroundprocess; i++)
		{
			if (background_pids[i] != 0)
			{
				//	printf("%s\n", background_name[i]);
				int no = background_pids[i];
				char *val = (char *)malloc(sizeof(char) * 100);
				char *cat = (char *)malloc(sizeof(char) * 100);
				int cnt = 0;
				while (no > 0)
				{
					val[cnt] = no % 10 + '0';
					no /= 10;
					++cnt;
				}

				--cnt;
				int j = 0;
				while (j <= cnt)
				{
					cat[j] = val[cnt - j];
					++j;
				}
				strcat(st, cat);
				strcat(st, "/stat");

				int fd = open(st, O_RDONLY);
				if (fd == -1)
				{
					printf("Background process 'PID' : %s exited successfully \n", cat);
					background_pids[i] = 0;
				}
				else
				{
					char *buff = (char *)calloc(1000, sizeof(char));
					read(fd, buff, 1000);
					char **parts = (char **)malloc(100 * sizeof(char *));
					int k = 0;
					while (k < 10)
					{
						parts[k] = strsep(&buff, " ");
						++k;
					}
					if (strcmp(parts[2], "Z") == 0)
					{
						// printf("%d\n", i);
						printf("Background process %s of 'PID' : %s exited successfully \n", parts[1], cat);
						background_pids[i] = 0;
					}
					close(fd);
				}
				free(val);
				free(cat);
			}
		}
		// free(st);
		init_shell();
	}

	return 0;
}