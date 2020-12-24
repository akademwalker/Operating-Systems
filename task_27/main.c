#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#include <wait.h>

#define MAXBUFF 64

void printStat(int stat)
{
	if(WCOREDUMP(stat))
		printf("Process ended with core dump %d\n", WCOREDUMP(stat));
	if(WEXITSTATUS(stat))
		printf("Process ended with exit code %d\n", WEXITSTATUS(stat));
	if(WIFCONTINUED(stat))
		printf("Process continued %d", WIFCONTINUED(stat));
	if(WIFSIGNALED(stat))
		printf("Process terminated due to signal", WIFSIGNALED(stat));
	if(WIFSTOPPED(stat))
		printf("Process currently stopped\n");
	if(WSTOPSIG(stat))
		printf("Process stopped due to signal %d", WSTOPSIG(stat));
	if(WTERMSIG(stat))
		printf("Process terminated due to signal %d", WTERMSIG(stat));
}

int main()
{
	FILE* in = popen("grep -E '^( )*$' main.c | wc -l", "r");
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
	} else if(!WIFEXITED(stat))
	{
		printStat(stat);
		return 1;
	}

	printf("Blank lines num: %u\n", atol(buff));
	return 0;
}
