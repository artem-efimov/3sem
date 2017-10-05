#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

typedef struct 
{
	int par[2];
	int ch[2];
} dpipe_t;


int main()
{	
	dpipe_t fd;
	pid_t result;
	int status;
	int p1=pipe(fd.par);
	int p2=pipe(fd.ch);
	
	char string1[4096], string2[4096], resstring1[4096], resstring2[4096];
	int i=0, k=0;
	size_t size;
	
	

	if(p1<0||p2<0)
	{
		printf("ERROR in create pipe\n");
	}
	
	printf("What will parent send to child? - ");
	while((string1[i]=getchar())!='\n')
	{
		i++;	
	}
	string1[i]='\0';
	printf("What will child send to parent? - ");
	while((string2[k]=getchar())!='\n')
	{
		k++;
	}
	string2[k]='\0';

	result=fork();
	
	if(result<0)
	{
		printf("Cannot fork child");
		exit(-1);
	}
	if(result)// parent begin
	{

					
		
		size = write(fd.par[1], string1, i);
		if(size!=i)
		{
			printf("Cannot write all parent' string\n");
			exit(-1);
		}

		close(fd.par[1]);
		
		
			
		waitpid(result, &status, 0);
		
		
		size = read(fd.ch[0], resstring1, k);
		if(size<0)
		{
			printf("Cannot read child' string\n");
			exit(-1);
		}
		
		printf("I'm parent and I receive from child  -  %s\n", resstring1);
		close(fd.ch[0]);//parent end
	}
	else//child begin
	{					
	
		size=write(fd.ch[1], string2, k);
		if(size!=k)
		{
			printf("Cannot write all child' string");
			exit(-1);
		}
		
		size=read(fd.par[0], resstring2, i);
		if(size<0)
		{
			printf("Cannot read parent' string\n");
			exit(-1);
		}
		
	
	
		printf("I'm child and I receive from parent -  %s\n", resstring2);
		close(fd.ch[1]);
		close(fd.par[0]);
	}
	
	return 0;
}
