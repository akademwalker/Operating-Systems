#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

int main() {
	if( putenv("TZ=America/Los_Angeles") != 0)
	{
		printf("Error in timezone set\n");
		exit(1);
	}

	time_t now;

	(void) time(&now);

	printf("%s\n", ctime(&now));

	return 0;
}

