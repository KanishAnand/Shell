default: main

main: main.c execute_command.c init_shell.c ls_implement.c wait_input.c system_commands.c pinfo.c nightswatch.c main.h history.c execute_command.h init_shell.h ls_implement.h wait_input.h system_commands.h pinfo.h history.h nightswatch.h
	gcc main.c execute_command.c init_shell.c ls_implement.c wait_input.c system_commands.c pinfo.c history.c nightswatch.c -g -Wall -Wextra