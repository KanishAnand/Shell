# Shell
This is a Linux Shell implemented in C language with some inbuilt commands and some user-defined commands.It also has functionalities to run both foreground and background process.It supports semicolon separated commands too. Also piping and redirection is implemented in the shell. Proper care is taken for Error Handling and Signal Handling too.

# Functionalities
1. `echo [arguments]` 
Print arguments on the shell.
2. `pwd`
Shows the current working directory path.
3. `history`
Shows the last 10 commands.
4. `pinfo <pid>`
Prints details of the process with given pid.If no argument is provided it takes pid of current process by default.
Details include : PID, Process Status, Memory , Executable Path.
5. `cd`
Changes the directory
  >* cd [argument]
  Changes the current directory to the directory mentioned in argument.
  >* cd ..
  Takes back to the previous directory.
  >* cd or cd ~
  Take to the home direcotry.
  >* cd /
  Takes to root directory.
6. `ls`        
Lists all contents of current directory.       
>* `ls -a`        
Lists all hidden files also.
>* `ls -l`
Long Listing of all files.      
>* `ls -al`       
Long Listing of all files including hidden files.
7. `setenv [var] [val]`
Sets value of the environment variable 'var' to value 'val'.
8. unsetenv [var]
Unsets the value of environment variable 'var'.
9. `jobs`
Prints all the jobs in order of their creation along with their Unique Id,current status, Name of the job, Pid.
10.`fg [job_id]`
Brings the Job with given job_id to the foregound process i.e brings stopped or sleeping process to foreground.
11. `bg[job_od]`
Takes the job with given job_id to the background process.
12. `Ctrl-Z`
Changes status of currently running job to stop and takes it to the background.
13. `Ctrl-C`
Interrupts the running process.
14. `Overkill`
Kills all jobs at once.
15 `kjob [job_id] [Signal_number]`
Sends signal 'signal_number' to process with given job_id.
16. `cronjob -c <command name> -t <time_interval> -p <total period of running>`
eg. cronjob -c ls -t 3 -p 6
This command should execute `ls` command after every 3 seconds until 6 seconds
are elapsed.
17. `nightswatch <number> <command>`
This is a modified version of watch. Press 'q' to exit.
`nightswatch -n 5 interrupt`
print number of times CPU has been interrupted by the keyboard.
`nightswatch -n 1 dirty`
print size of the part of the memory which is dirty
18.`quit`
Exits the shell.
19. Handles multiple piping and redirection properly.
20. UP arrow will results in running of previous nth number command where n is number of times up arrow is pressed.

# Usage
To run the shell
```
make
./a.out
```
To quit from the shell
```
quit
```
