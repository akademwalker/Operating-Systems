#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void printUserID()
{
	printf("Real user ID: %lu\nEffective user ID: %lu\n",
		(unsigned long int)getuid(),
		(unsigned long int)geteuid()
		);
}

void userOpenFile(const char* filename)
{
	printUserID();

	FILE* fp = fopen(filename, "r");

	if (fp == NULL)
	{
		perror("Cannot open file\n");
		return;
	}

	printf("File %s opened\n", filename);
	fclose(fp);
}


int main() {
	printf("Attempt no.1:\n");
	userOpenFile("text.txt");
	printf("\n");

	//setuid
	errno = 0;
	if(setuid(getuid()) == -1)
	{
		perror("Cannot set effecive user ID\n");
		return 1;
	}
	printf("Real ID and effective ID set equal\n\n");

	printf("Attempt no.2:\n");
	userOpenFile("text.txt");

	return 0;
}
