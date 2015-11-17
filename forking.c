/*#include <unistd.h>
#include <stdio.h>

int main(void)
{
    	char *argv[] = { "/project/sh", "-c", "genmap",  0};
    	char *envp[] =
    	{
        	"HOME=/",
        	"PATH=/project:/project/LinuxProject",
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
}*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = {"/genmap.sh",  "sh", 0 };
    char *envp[] =
    {
        "USER=eric",
	"PATH=/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/eric/bin",
        0
    };
    execve(argv[0], argv, envp);
	fprintf(stderr, "Oops!\n");
    return -1;
}
