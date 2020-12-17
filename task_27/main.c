#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#define MAXBUFF 64

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
	if(pclose(in) == -1)
	{
		perror("Error in closing pipe");
		return 1;
	}

	printf("Blank lines num: %u\n", atol(buff));
	return 0;
}
