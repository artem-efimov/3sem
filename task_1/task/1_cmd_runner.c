/*
  Write a program which allow you to: 
- run another programs via command line.
- get exit codes of terminated programs

## TIPS:
1. Use "2_fork_wait_exit.c" and "4_exec_dir.c" from examples. Combine them.
2. Parse input string according to the type of exec* (see "man exec").
   a) if execvp is used, string splitting into "path" and "args" is all you need.
3. Collect exit codes via waitpid/WEXITSTATUS.
*/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

char* ChangWords(char* data ) 
{
	int i = 0;
	int s = 0;
	int k = 0;
	char* arguments;
	while (data[s] == ' ') 
	{
		s++;
	}
	k = s;
	while (data[k] != ' ' && data[k] != '\0' && data[k]!='\n' )
	{
		k++;
	}

	arguments = (char*)malloc((k - s + 1)*sizeof(char));

	i = k;

	arguments[i - s] = '\0';	

	while (--i >= s)
	{
		arguments[i - s] = data[i];
	}
	s = 0;

	while (data[k] != '\0') 
	{
		data[s++] = data[k++];
	}
	data[s] = '\0';

	return arguments;	
}


int strln(char* massiv)
{
	int len = 0;
	int i = 0;
	char prepare = 1;
	while ((massiv[i] = getchar()) != EOF) 
	{		
		if(massiv[i] == '\n')
		{
			massiv[i] = '\0';
			break;
		}
		if (massiv[i] == ' ') 
		{
			prepare = 1;
		}
		else {
			if (prepare == 1)
			{
				prepare == 0;
				len++;
			} 
		}
		i++;
	}
	return len;
}

char** distribution(int N, char* massiv)
{
	int i;
	char** a;
	a = (char**)malloc((N+1)*sizeof(char*));
	for (i = 0; i < N; i++) {
		a[i] = ChangWords(massiv);
	}
	a[N] = '\0'; 
	return a;
}


int main() 
{
	int status;
	char Prepare = 1;
	int len;
	char massiv[100];
	char** argv;
	pid_t pid = fork();
	while (Prepare) 
	{

		switch(pid)
		{	
			case -1: printf("ERROR in start fork");
				 exit -1;	

			

			case 1:  waitpid(pid, &status, 0);
				 printf("Ret code - %d\n Push 'N'\n", WEXITSTATUS(status));

			
				 if(getchar() == 'N')
				 {	
					 Prepare = 0;
				 }

			default:	
				 len = strln(massiv);
				 argv = distribution(len, massiv);		
				 execvp(argv[0], argv);
		} 
	}	
	return 0;
}
