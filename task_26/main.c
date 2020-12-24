#include <unistd.h>
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

void makeUpper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)

		buff[i] = toupper(buff[i]);
}

int main()
{
	FILE* in = popen("cat input.txt", "r");
	if(in == NULL)
	{
		perror("Error in opening pipe");
		return 1;
	}


	size_t count;
	char buff[MAXBUFF];
	while(1)
	{
		count = fread(buff, sizeof(char), MAXBUFF, in);
		if(ferror(in))
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

	makeUpper(buff, count);

	while(1)
	{
		fwrite(buff, sizeof(char), count, stdout);
		if(ferror(stdout))
		{
			if(errno == EINTR)
			{
				errno = 0;
				continue;
			}
			perror("Error in writing to stdout");
			return 1;
		}
		break;
	}
	return 0;
}
