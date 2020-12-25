#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <wait.h>

#define MAXBUFF 64

int checkStat(int stat)
{
	if(WIFEXITED(stat))
	{
		int exitCode = WEXITSTATUS(stat);
		if(exitCode !=0)
		{
			printf("Process exited with exit code", exitCode);
			return -1;
		}
	}
	else if(WIFSIGNALED(stat))
	{
		printf("Process terminated due to signal", WTERMSIG(stat));
		return -1;
	}
	else if(WIFSTOPPED(stat))
	{
		printf("Process stopped due to signal %d", WSTOPSIG(stat));
		return -1;
	}

	return 0;
}

int main()
{
	FILE* in = popen("grep -E '^$' main.c | wc -l", "r");
	if(in == NULL)
	{
		perror("Error in opening pipe");
		return 1;
	}

	char buff[MAXBUFF];
	char* numLines;

	while(1)
	{
		numLines = fgets(buff, MAXBUFF, in);
		if(numLines == NULL)
		{
			if(errno == EINTR)
			{
				errno = 0;
				continue;
			}
			perror("Error in reading from pipe");
			if(pclose(in) == -1)
				perror("Error in closing pipe");
			return 1;
		}
		break;
	}


	int stat = pclose(in);
	if(stat == -1)
	{
		perror("Error in closing pipe");
		return -1;
	} else if(checkStat(stat))
	{
		return -1;
	}

	printf("Blank lines num: %u\n", atol(buff));
	return 0;
}
