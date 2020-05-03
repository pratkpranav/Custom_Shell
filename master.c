#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <signal.h>
#include<stdbool.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<fcntl.h>

int main()
{
		pid_t cpid = fork();

		if(cpid == 0)
		{	
			printf("%s\n", "            ::               SHELL                :: " );
			char *args[] = {"./shell.exe", NULL};
			execvp(args[0], args);
		}	
		else
		{

    		waitpid(cpid, NULL, WUNTRACED);

		}
}
