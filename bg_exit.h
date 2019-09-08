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
int bg_exit()
{
	int pid,pid_status;
	while((pid = waitpid(-1,&pid_status,WNOHANG|WUNTRACED))>0)
	{
		if(WIFEXITED(pid_status)){
			for(int i=0;i<count;i++)
			{
				if(bg1[i] == pid)
				{
					bg1[i] = -1;
					//running_proc_size--;		
					// char *t3;
					// t3 = get_bgname(pid);
					fprintf(stderr,"%s with pid %d exited normally \n",names[i],pid);
					//free(running[i].pname);
					break;
				}
			}
		}
		else if(WIFSIGNALED(pid_status))
		{
			for(int i=0;i<count;i++)
			{
				if(bg1[i] ==pid)
				{
					bg1[i] = -1;
					//running_proc_size--;
					fprintf(stderr,"process with pid %d terminated \n",pid);
					//free(running[i].pname);
					break;
				} 
			}
		}
	}	
}

