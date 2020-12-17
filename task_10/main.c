#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void printStat(int pid, int stat)
{
	printf("Process status - %d\n", stat);
	if(WCOREDUMP(stat))
		printf("Process %d ended with core dump %d\n", pid, WCOREDUMP(stat));
	if(WEXITSTATUS(stat))
		printf("Process %d ended with exit code %d\n", pid, WEXITSTATUS(stat));
	if(WIFCONTINUED(stat))
		printf("Process %d ended with continued %d\n", pid, WIFCONTINUED(stat));
	if(WIFEXITED(stat))
		printf("Process %d terminated normally with %d\n", pid, WIFEXITED(stat));
	if(WIFSIGNALED(stat))
		printf("Process %d terminated due to signal %d\n", pid, WIFSIGNALED(stat));
	if(WIFSTOPPED(stat))
		printf("Process %d currently stopped\n", pid);
	if(WSTOPSIG(stat))
		printf("Process %d stopped due to signal %d\n", pid, WSTOPSIG(stat));
	if(WTERMSIG(stat))
		printf("Process %d terminated due to signal %d\n", pid, WTERMSIG(stat));
}

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		printf("usage: ./prog [cmd] [arg]\n");
		return 1;
	}

	int pid;
	pid = fork();

	if (pid == -1)
	{
		perror("Fork failure");
		return 1;
	}

	//child
	if(pid == 0)
	{
		execvp(argv[1], argv + 1);
		perror("Execute argv error");
		return 1;
	} else
	//parent
	{
		int childStat;
		while(wait(&childStat) == -1)
		{
			if (errno == EINTR)
			{
				errno = 0;
				continue;
			}
			perror("Wait failure");
			return 1;
		}
		//printf("Child %d endcode - %d\n", pid, childStat);
		printStat(pid, childStat);
	}

	return 0;
}
