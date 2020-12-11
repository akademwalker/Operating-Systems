#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int pid;
	pid = fork();

	if (pid == -1)
	{
		perror("Fork failure");
		return 1;
	}

	//child
	if (pid == 0)
	{
		if(execlp("cat", "cat", "text.txt", (char*) 0) == -1)
		{
			perror("Execute cat failure");
			return 1;
		}
	} else
	//parent
	{
		int childStat;
		printf("Parent begin\n");

		errno = 0;
		while(wait(&childStat) == -1)
		{
			if(errno == EINTR)
			{
				errno = 0;
				continue;
			}
			perror("Wait failure");
			return 1;
		}

		printf("Parent end\n");
	}
	return 0;
}
