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

#define clear() printf("\033[H\033[J")
//Global Variables
char home_dir[1024];

void execute_command(char *command, char *arg, int no_of_args);
void ls_implement(char *command, char *arg, int no_of_args);
void printFileProperties(struct stat stats);

void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	int len = strlen(home_dir);
	printf("~");
	for (int i = len; i < strlen(cwd); i++)
	{
		printf("%c", cwd[i]);
	}
	printf(">");
}

void init_shell()
{
	struct utsname buffer;
	printf("***");
	char *user = getenv("USER");
	printf("<%s@:", user);
	//printf("%s:",(&buffer)->sysname);
	printDir();
	// sleep(1);
}

void wait_input()
{
	int length = 200;
	int length_2d = 200000;
	char *buff = (char *)malloc(length * sizeof(char));
	fgets(buff, length, stdin);
	//char *part1,*part2;
	//char *parts = (char*)malloc(length_2d*sizeof(char));
	char **parts = (char **)malloc(40 * sizeof(char *));
	for (int i = 0; i < 5; i++)
		parts[i] = (char *)malloc(400 * sizeof(char));

	int i = 0;
	while (1)
	{
		parts[i] = strsep(&buff, " ");
		int l = strlen(parts[i]);
		if (parts[i][l - 1] == '\n')
		{
			break;
		}
		++i;
	}
	// part1 = strsep(&buff," ");
	// part2 = strsep(&buff," ");
	// int l = strlen(part2);
	//this is done because fgets puts a new line character at end of read line so when we pass it to exectue command as arg then chdir(arg) was not working.
	// part2[l-1] = '\0';
	// printf("%d\n",i);
	int l = strlen(parts[i]);
	parts[i][l - 1] = '\0';
	execute_command(parts[0], parts[1], i);
}

void execute_command(char *command, char *arg, int no_of_args)
{
	char cwd[1024];
	if (strcmp(command, "cd") == 0)
	{
		chdir(arg);
		init_shell();
	}
	else if (strcmp(command, "echo") == 0)
	{
		printf("%s\n", arg);
		init_shell();
	}
	else if (strcmp(command, "pwd") == 0)
	{
		char cwd[1024];
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
		init_shell();
	}
	else if (strcmp(command, "ls") == 0)
	{
		ls_implement(command, arg, no_of_args);
		init_shell();
	}
}

void ls_implement(char *command, char *arg, int no_of_args)
{
	DIR *p;
	struct dirent *d;
	struct stat stats;
	// printf("%d\n", no_of_args);
	// printf("%s\n", arg);
	// printf("%ld\n", strlen(arg));
	if (no_of_args == 0 || strlen(arg) == 0)
	{
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		arg = cwd;
	}
	p = opendir(arg);
	if (p == NULL)
	{
		perror("Cannot find directory");
	}
	else
	{
		while (d = readdir(p))
		{
			if (stat(d->d_name, &stats) == 0)
			{
				printFileProperties(stats);
			}
			printf("%s ", d->d_name);
			printf("\n");
		}
		printf("\n");
	}
}

void printFileProperties(struct stat stats)
{
	struct tm dt;

	// File permissions
	// printf("\nFile access: ");
	if (stats.st_mode & R_OK)
		printf("r");
	if (stats.st_mode & W_OK)
		printf("w");
	if (stats.st_mode & X_OK)
		printf("x ");

	// File size
	printf("%ld ", stats.st_size);

	// Get file creation time in seconds and
	// convert seconds to date and time format
	dt = *(gmtime(&stats.st_ctime));
	printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
	// File modification time
	dt = *(gmtime(&stats.st_mtime));
	//printf("%d-%d-%d %d:%d:%d ", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
	//  dt.tm_hour, dt.tm_min, dt.tm_sec);
}

int main()
{
	clear();
	getcwd(home_dir, sizeof(home_dir));
	init_shell();
	while (1)
	{
		// printDir();
		wait_input();
	}
	return 0;
}