#include <unistd.h>
#include <stdio.h>

int main(void)
{
    	char *argv[] = {"/bin/sh", "genmap.sh", 0 };
   	char *envp[] =
    	{
       		 "PATH=/bin",
       	 	0
	};

	int pid = fork();
	if (pid == 0)
	{
		printf("maybe was called");
    		execve(argv[0], &argv[0], envp);
    	}
	else if (pid > 0)
	{
		printf("forked successfully");
	}
	else
	{
		fprintf(stderr, "Oops!\n");
    		return -1;
	}
	return 0;
}
