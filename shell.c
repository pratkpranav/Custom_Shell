// C Program to design a shell in Linux 
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

void pwd(char pwd [])
{


    char cwd[1024];
    chdir("/path/to/change/directory/to");
    getcwd(cwd, sizeof(cwd));
    strcpy(pwd, cwd);
}








int main(int argc, char **argv) 
{ 
	 


	while(1)
	{


	char* inputi;
	char* pit;
	inputi = readline("\nshell>");
	pit = inputi;
	//printf("%s\n", pit);
	char copyinput[strlen(inputi)];
	//printf("%s\n", inputi);
	for(int i=0; i<strlen(inputi) ; i++)
	{
		copyinput[i] = inputi[i];
	}

	//printf("%s\n", inputi);

	char* input = strtok(inputi, " ");


	//printf("%s\n", inputi);
	if(!strcmp(input, "pwd"))
	{

		char pwdir[1024];
		pwd(pwdir);
		printf("%s\n", pwdir );

	}


	else if(!strcmp(input, "exit"))
	{
		return 0;

	}

	else if(!strcmp(input, "mkdir"))
	{
		char *name = strtok(NULL, " ");
		int check;  
    	//clrscr(); 
  
    	check = mkdir(name , 0700); 
  
    	if (!check) 
    	    printf("Directory created\n"); 
    	else 
    	{ 
    	    printf("Unable to create directory\n"); 
    	    //exit(1); 
    	} 
	}


	else if(!strcmp(input, "rmdir"))
	{
		char *name = strtok(NULL, " ");
		int status;

		status = rmdir(name);


		if (!status) 
    	    printf("Directory deleted\n"); 
    	else 
    	{ 
    	    printf("Unable to delete directory\n"); 
    	    //exit(1); 
    	} 

	}

	else if(!strcmp(input, "cd"))
	{
		char *name = strtok(NULL, " ");
		//printf("%s\n", name );
		int status = chdir(name);

		if (!status) 
    	    printf("Directory changed\n"); 
    	else 
    	{ 
    	    printf("Unable to change directory\n"); 
    	    //exit(1); 
    	} 



	}
	else
	{
		//printf("%s\n",  pit);
		//char* pch = strtok(pit , " ");
	char* argument[100];
	char* comm[100];
	int i=0;
	//printf("%s\n", "flag1");
	//argument[0] = input;
	//i++;
	int countcom = 0;
	while(input!= NULL)
	{
		i++;
		if(i%2==1){
			argument[i/2] = input;
			//printf("%s\n",pch );
		}
		else{
			comm[(i-1)/2] = input;
			countcom++;
			//printf("%s\n", );
		}

		input  = strtok(NULL, " ");
	}
	bool b = true;
	for(int i=0; i<countcom ; i++)
	{
		if(comm[i][0] != '|')
		{
			b = false;
		}
	}
	if(b && countcom > 1)
	{
		//printf("%s\n", "flag1");
		

			int ret;
			int mypipefd[2];
			ret = pipe(mypipefd);

			if(ret == -1)
			{
				perror("pipe");
			}

		for(int i=0; i<countcom ; i++)
		{	
			pid_t pid;

			//char buf[20];

			
			//printf("%s\n", "flag1" );
			pid = fork();
			if(pid == 0)
			{

				pid_t pidd = fork();

				if(pidd == 0)
				{
					if(i==countcom - 1)
					{	
						close(mypipefd[1]);
						dup2(mypipefd[0], 0);
						execlp(argument[i+1] , argument[i+1] , (char*)NULL);
					}
					else
					{	
						//close(mypipefd[1]);
						dup2(mypipefd[0], 0);
						dup2(mypipefd[1], 1);
						execlp(argument[i+1] , argument[i+1] , (char*)NULL);

					}
				}
				else
				{
					//printf("%s\n", "Child Process");
					if(i ==0)
					{
						close(mypipefd[0]);
						dup2(mypipefd[1], 1);
						execlp(argument[i] , argument[i] , (char*)NULL);

					}
					else
					{
						dup2(mypipefd[0],0);
						dup2(mypipefd[1], 1);
						execlp(argument[i] , argument[i] , (char*)NULL);

					}
				}

				
			}
			else
			{
    			waitpid(pid, NULL, WUNTRACED);

			}
		}
	}

	//printf("%d\n",  i);
	if(i == 1)
	{
		//printf("%s\n", "ll" );
		//printf("%s\n", argument[0] );


		pid_t cpid;
		cpid  = fork();
		if(cpid == 0)
		{	
			char *args[] = {argument[0], NULL};
			execvp(args[0], args);
		}	
		else
		{

    		waitpid(cpid, NULL, WUNTRACED);

		}
	}
	else
	{

	//printf("%s\n", "flag2");

		int k = 0;
		if(i==3)
		{
			//printf("%s\n", comm[0]);
			if(!strcmp(comm[0], ">"))
			{
	//printf("%s\n", "flag3");

			pid_t cpid;
			int ofd;
			cpid = fork();

			if(cpid == 0)
			{

				ofd = creat(argument[1], 0644);
				dup2(ofd , 1);
				execlp(argument[0], argument[0], (char*)NULL);
			}
			else
			{
    			//waitpid(cpid, NULL, WUNTRACED);

			}

			}
			else if(comm[0][0] == '<')
			{
				int ofd;
				pid_t cpid;

				cpid = fork();

					if(cpid == 0)
					{


					ofd = open(argument[1],  O_RDONLY);
					if(ofd < 0)
					{

						printf("%s\n",  "No such file exist");
					}

					dup2(ofd, STDIN_FILENO);
					execlp(argument[0] , argument[0] , (char*)NULL);
				}
				else
				{

    				waitpid(cpid, NULL, WUNTRACED);
    				//kill(cpid, SIGCONT);
				}
			}
			else
			{
				//printf("%s\n",  "It's always coming here only");
				pid_t pid;
				int mypipefd[2];

				int ret;
				char buf[20];

				ret = pipe(mypipefd);

				if(ret == -1)
				{
					perror("pipe");
				}

				pid = fork();
				if(pid == 0)
				{

					pid_t pidd = fork();

					if(pidd == 0)
					{
						close(mypipefd[1]);
						dup2(mypipefd[0], 0);
						execlp(argument[1] , argument[1] , (char*)NULL);
					}
					else
					{
						//printf("%s\n", "Child Process");
						close(mypipefd[0]);
						dup2(mypipefd[1], 1);
						execlp(argument[0] , argument[0] , (char*)NULL);
					}

				
				}
				else
				{
    				waitpid(pid, NULL, WUNTRACED);

				}



			}
		}
		else
		{
			int ofd1, ofd;
			if(i==5)
			{
				if(comm[0][0] == '<' && comm[1][0] == '>')
				{
					int ofd;
					pid_t cpid;

					cpid = fork();

					if(cpid == 0)
					{


					ofd1 = open(argument[1],  O_RDONLY);
					ofd = creat(argument[2], 0644);
				
					if(ofd1 < 0)
					{

						printf("%s\n",  "No such file exist");
					}

					dup2(ofd1, STDIN_FILENO);
					dup2(ofd , 1);



					execlp(argument[0] , argument[0] , (char*)NULL);
					}
					else
					{

    					waitpid(cpid, NULL, WUNTRACED);
    					//kill(cpid, SIGCONT);
					}

				}
				else if(comm[0][0] == '>' && comm[1][0] == '<')
				{
					int ofd;
					pid_t cpid;

					cpid = fork();

					if(cpid == 0)
					{


					ofd1 = open(argument[2],  O_RDONLY);
					ofd = creat(argument[1], 0644);
				
					if(ofd1 < 0)
					{

						printf("%s\n",  "No such file exist");
					}

					dup2(ofd1, STDIN_FILENO);
					dup2(ofd , 1);



					execlp(argument[0] , argument[0] , (char*)NULL);
					}
					else
					{

    					waitpid(cpid, NULL, WUNTRACED);
    					//kill(cpid, SIGCONT);
					}

				}
				else if(comm[0][0] == '|' && comm[1][0] == '>')
				{
					pid_t pid;
					int mypipefd[2];

					int ret;
					char buf[20];

					ret = pipe(mypipefd);

					if(ret == -1)
					{
						perror("pipe");
					}

					pid = fork();
					if(pid == 0)
					{

						pid_t pidd = fork();

						if(pidd == 0)
						{
							close(mypipefd[1]);
							dup2(mypipefd[0], 0);
							ofd = creat(argument[2], 0644);
							dup2(ofd, 1);
							execlp(argument[1] , argument[1] , (char*)NULL);
						}
						//printf("%s\n", "Child Process");
						else
						{

						close(mypipefd[0]);
						dup2(mypipefd[1], 1);
						execlp(argument[0] , argument[0] , (char*)NULL);
						}

					}
					else
					{
    					waitpid(pid, NULL, WUNTRACED);

					}

					
				}
				else if(comm[0][0] == '<' && comm[1][0] == '|')
				{
					pid_t pid;
					int mypipefd[2];

					int ret;
					char buf[20];

					ret = pipe(mypipefd);

					if(ret == -1)
					{
						perror("pipe");
					}

					pid = fork();
					if(pid == 0)
					{

						pid_t pidd = fork();

						if(pidd == 0)
						{

							close(mypipefd[1]);
							dup2(mypipefd[0], 0);
							//ofd = creat(argument[3], 0644);
							//dup2(ofd, 1);
							execlp(argument[2] , argument[2] , (char*)NULL);
						}
						else
						{
							//printf("%s\n", "Child Process");
							close(mypipefd[0]);
							dup2(mypipefd[1], 1);

							ofd1 = open(argument[1],  O_RDONLY);
							dup2(ofd1 , 0);
							execlp(argument[0] , argument[0] , (char*)NULL);
						}

					}
					else
					{
    					waitpid(pid, NULL, WUNTRACED);

					}
				}

			}
			if(i==7)
			{
				if(comm[0][0] == '<' && comm[1][0] == '|' && comm[2][0] == '>')
				{
					pid_t pid;
					int mypipefd[2];

					int ret;
					char buf[20];

					ret = pipe(mypipefd);

					if(ret == -1)
					{
						perror("pipe");
					}

					pid = fork();
					if(pid == 0)
					{

						pid_t pidd =  fork();
						if(pidd == 0)
						{

							close(mypipefd[1]);
							dup2(mypipefd[0], 0);
							int ofd = creat(argument[3], 0644);
							dup2(ofd, 1);
							execlp(argument[2] , argument[2] , (char*)NULL);
						}
						else
						{
							//printf("%s\n", "Child Process");
							close(mypipefd[0]);
							dup2(mypipefd[1], 1);

							int ofd1 = open(argument[1],  O_RDONLY);
							dup2(ofd1 , 0);
							execlp(argument[0] , argument[0] , (char*)NULL);
						}

					}
					else
					{
    					waitpid(pid, NULL, WUNTRACED);

					}
				}
			}

		}

		// do nothing rn
	}
	}
}
	return 0; 
} 
