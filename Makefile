default: main

main: main.c execute_command.c init_shell.c ls_implement.c wait_input.c system_commands.c pinfo.c nightswatch.c redirection.c ioredirection.c jobs.c signal_handling.c main.h history.c signal_handling.h execute_command.h init_shell.h ls_implement.h wait_input.h system_commands.h pinfo.h history.h nightswatch.h redirection.h ioredirection.h jobs.h
	gcc main.c execute_command.c init_shell.c ls_implement.c wait_input.c system_commands.c pinfo.c jobs.c history.c nightswatch.c redirection.c ioredirection.c signal_handling.c -g3 -Wall -Wextra


