#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <ulimit.h>
#include <sys/resource.h>

void printUserGroupID()
{
	printf("UID: %u\n", getuid());
	printf("EUID: %u\n", geteuid());
	printf("GID: %u\n", getgid());
	printf("EGID: %u\n", getegid());
}

void printProcessID()
{
	errno = 0;
	pid_t pid = getpid();
	pid_t ppid = getppid();
	pid_t  pgrp = getpgrp();

	if(errno != 0)
	{
		perror("Error in getting process ID");
		return;
	}

	printf("Process ID: %u\n", pid);
	printf("Process-parent ID: %u\n", ppid);
	printf("Process group ID: %u\n", pgrp);
}

void makeProcGrpLeader()
{
	errno = 0;
	pid_t currPGID = getpgrp();

	setpgid(0, currPGID);

	if(errno != 0)
	{
		perror("Error: cannot make process leader of group");
		return;
	}

	printf("Process became leader of group\n");
}

void printUlimit()
{
	errno = 0;
	struct rlimit rlim;
	getrlimit(RLIMIT_FSIZE, &rlim);

	if(errno !=0)
	{
		perror("Error in ulimit");
		return;
	}

	printf("Ulimit:\nsoft - %ju\nhard - %ju\n", rlim.rlim_cur, rlim.rlim_max);
}

void setUlimit(char* strUlimit)
{
	errno = 0;
	long newUlimit = atol(strUlimit);

	if (newUlimit <= 0)
	{
		printf("Error: invalid arg to set ulimit\n");
		return;
	}

	struct rlimit rlim = {newUlimit, newUlimit};
	setrlimit(RLIMIT_FSIZE,&rlim);

	if (errno != 0)
	{
		perror("Error: cannot set ulimit");
		return;
	}
}

void printCoreLimit()
{
	errno = 0;
	struct rlimit rlim;
	getrlimit(RLIMIT_CORE, &rlim);

	if(errno !=0)
	{
		perror("Error in core limit");
		return;
	}

	printf("Core Limit:\nsoft - %ju\nhard -  %ju\n", rlim.rlim_cur, rlim.rlim_max);
}

void setCoreLimit(char* strCoreLimit)
{
	errno = 0;
	long newCoreLimit = atol(strCoreLimit);

	if (newCoreLimit <= 0)
	{
		printf("Error: invalid arg to set core-file size limit\n");
		return;
	}

	struct rlimit rlim = {newCoreLimit, newCoreLimit};
	setrlimit(RLIMIT_CORE,&rlim);

	if (errno != 0)
	{
		perror("Error: cannot set core-file size limit");
		return;
	}
}

void printCurrDir()
{
	char const* path = getenv("PWD");

	if(path != NULL)
	{
		printf("Error in printing current directory path\n");
		return;
	}

	printf("Current directory path: %s\n", path);
}

void printEnvVar(char** envp)
{
	for(size_t i = 0; envp[i] != NULL; ++i)
	{
		printf("%s\n", envp[i]);
	}
}

void updateEnvVal(char * valStr)
{
	errno = 0;
	putenv(valStr);
	if(errno != 0)
	{
		perror("Error in updating enviroment variables");
	} else
	{
		printf("Enviroment variables updated with: %s", valStr);
	}
}

int main(int argc, char**  argv, char** envp) {

	if (argc == 1) {
		printf("no arguments\n");
		return 0;
	}

	int currOpt = 0;
	while (optind < argc)
	{
		if ((currOpt = getopt(argc, argv, "ispuU:cC:dvV:")) != -1)
		{
			switch(currOpt)
			{
				case 'i':
					printUserGroupID();
					break;
				case 's':
					makeProcGrpLeader();
					break;
				case 'p':
					printProcessID();
					break;
				case 'u':
					printUlimit();
					break;
				case 'U':
					setUlimit(optarg);
					break;
				case 'c':
					printCoreLimit();
					break;
				case 'C':
					setCoreLimit(optarg);
					break;
				case 'd':
					printCurrDir();
					break;
				case 'v':
					printEnvVar(envp);
					break;
				case 'V':
					updateEnvVal(optarg);
					break;
				//case ':':
				//	fprintf(stderr, "-%c: option argument missing\n", optopt);
				//	break;
				//case '?':
				//	fprintf(stderr, "-%c: option not specified\n", optopt);
				//	break;
				default:
					printf("Error: unknown behaviour\n");
					return 1;
			}
		}
		else
		{
			printf("No options to parce\n");
			return 1;
		}
	}
	return 0;
}
