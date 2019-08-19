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
#include <string.h>
#include "wait_input.h"
#include "init_shell.h"
#include "ls_implement.h"
#include "execute_command.h"

#define clear() printf("\033[H\033[J")
//Global Variables
char home_dir[1024];

int main()
{
	clear();
	getcwd(home_dir, sizeof(home_dir));
	init_shell();
	while (1)
	{
		wait_input();
	}
	return 0;
}