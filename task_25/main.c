#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>


#define MAXBUFF 64

int writeToPipe(int writefd, char const* msg, int len)
{
	errno = 0;
	int count;
	while((count = write(writefd, msg, len)) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		} else
		{
			perror("Error in writing to pipe");
		}
		return count;
	}
}

int readFromPipe(int readfd, char* buff)
{
	errno = 0;
	int count;
	while(count = read(readfd, buff, MAXBUFF)== -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		} else
		{
			perror("Error in reading from pipe");
		}
		return count;
	}
}

void makeUpper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

int closePipe(int pfildes[2])
{
	errno = 0;
	if(close(pfildes[0]) || close(pfildes[1]) == -1)
	{
		perror("Error in closing pipe");
		return -1;
	}
	return 0;
}


int main()
{
	int pfildes[2];
	const char* msg = "Hello, world!\n";
	int len = strlen(msg) + 1;

	if(pipe(pfildes) == -1)
	{
		perror("Error in initializing pipe");
		return -1;
	}

	pid_t wpid = fork();
	if(wpid == -1)
	{
		perror("Error in fork");
		closePipe(pfildes);
		return -1;
	}

	if(wpid == 0)
	{
		if(write(1, msg, len) != len)
		{
			perror("Error in stdout in write child process");
			return -1;
		}
		return writeToPipe(pfildes[0], msg, len);
	}
	int wstat;
	while(wait(&wstat) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		perror("Error in wait");
		return 1;
	}

	pid_t rpid = fork();
	if(rpid == -1)
	{
		perror("Error in fork");
		closePipe(pfildes);
		return -1;
	}

	if(rpid == 0)
	{
		char buffer[MAXBUFF];
		readFromPipe(pfildes[1], buffer);
		makeUpper(buffer, len);
		if(write(1, buffer, len) != len)
		{
			perror("Error in stdout print in read child process");
			return -1;
		}
		return 0;
	}

	int rstat;
	while(wait(&rstat) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		perror("Error in read");
		return 1;
	}

	closePipe(pfildes);
	return 0;
}
