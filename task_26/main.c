#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>


#define MAXBUFF 64

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
	if(pclose(in) == -1)
	{
		perror("Error in closing pipe");
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
