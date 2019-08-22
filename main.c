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

#define clear() printf("\033[H\033[J")
//Global Variables
char home_dir[1024];
int background_pids[200];
int no_of_backgroundprocess = 0;

int main()
{
	clear();
	getcwd(home_dir, sizeof(home_dir));
	init_shell();

	while (1)
	{
		wait_input();
		char *st = (char *)malloc(100 * sizeof(char));
		strcat(st, "/proc/");
		for (int i = 0; i < no_of_backgroundprocess; i++)
		{
			if (background_pids[i] != 0)
			{
				//printf("%d\n", background_pids[i]);
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
				//printf("%s\n", cat);
				strcat(st, cat);
				strcat(st, "/stat");
				int fd = open(st, O_RDONLY);
				if (fd == -1)
				{
					printf("Background process of PID : %s exited successfully \n", cat);
					background_pids[i] = 0;
				}
				else
				{
					close(fd);
				}
			}
		}

		init_shell();
	}

	return 0;
}