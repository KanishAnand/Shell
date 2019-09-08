#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<fcntl.h> 
#include <dirent.h>
#include<sys/types.h> 
#include<sys/wait.h>
#include<grp.h>
#include<pwd.h>
#include <sys/stat.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
void execArgs(char** parsed,int bg) 
{  
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 

        if(bg == 1)
        {
             setpgid(0,0);
             close(STDIN_FILENO);
             close(STDOUT_FILENO);
             close(STDERR_FILENO);
        }
        if (execvp(parsed[0], parsed) < 0)  
            printf("\nCould not execute command.."); 
        
        exit(0); 
    } else { 
       if(bg ==0)
        	wait(NULL);
        else
        {
            bg1[count] = pid;        
            for(int w = 0;w<strlen(parsed[0]);w++)
             {
                 names[count][w] = parsed[0][w];
             }
            count+=1;
        }  
        return ; 
    } 
    // FOREGROUND -> using a fork() in which the parent process waits() on the child,and the child execvp()'s the program.
    // BACKGROUND -> The way to duplicate the behaviour of & in existing shells is to call fork(), then in the child call setpgid(0, 0);
    // to put the child into a new process group.
    // The parent just continues on (perhaps after printing the PID of the child) - it doesn't call wait().
} 