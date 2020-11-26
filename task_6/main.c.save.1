#include <stdio.h> //printf, errno
#include <sys/types.h> //open, lseek
#include <sys/stat.h> //open
#include <fcntl.h> //open
#include <unistd.h> //read, lseek, close
#include <stdlib.h> //malloc
#include <errno.h> //errno
#include <stdbool.h> //bool
#include <string.h> //strchr
#include <poll.h>
#include <fcntl.h> //fcntl

#define INC_COEF 1.5
#define DEFAULT_CAPACITY 8
#define BUFF_SIZE 16
#define TIMEOUT 5000

typedef struct _IndentTable
{
	off_t* arr;
	size_t size;
	size_t cap;
} IndentTable;


void initIndentTable(IndentTable* table)
{
	errno = 0;

	table->arr = calloc(DEFAULT_CAPACITY, sizeof(off_t));
	if(table->arr == NULL)
	{
		perror("Cannot allocate memory for ident array");
		return;
	}

	table->size = 0;
	table->cap = DEFAULT_CAPACITY;
}

bool resizeTable(IndentTable* table)
{
	errno = 0;

	off_t* oldArr = table->arr;
	size_t oldCap = table->cap;

	size_t newCap = oldCap * INC_COEF;

	table->arr = realloc(table->arr, newCap * sizeof(off_t));
	if (table->arr == NULL)
	{
		table->arr = oldArr;
		perror("Cannot realloc memory");
		return false;
	}
	table->cap = newCap;

	return true;
}

bool pushIndent(IndentTable* table, off_t val)
{
	if (table->size == table->cap)
	{
		if(!resizeTable(table))
		{
			return false;
		}
	}

	table->arr[table->size] = val;
	++table->size;
	return true;
}

bool fillIndentTable(IndentTable* table, int fildes)
{
	char buff[BUFF_SIZE + 1] = {0};
	off_t currPos = 0;
	ssize_t readCount = 0;

	int oldFl = fcntl(fildes, F_GETFL);
	fcntl(fildes, F_SETFL, oldFl | O_NONBLOCK);

	while((readCount = read(fildes, buff, BUFF_SIZE)) != 0)
	{
		if(readCount == -1)
		{
			fcntl(fildes, F_SETFL, oldFl);
			perror("Error in reading file");
			return false;
		}

		buff[readCount] = '\0';
		char* endlinePos = buff;

		while((endlinePos = strchr(endlinePos, '\n')) != NULL)
		{
			if(!pushIndent(table, currPos + (endlinePos - buff)))
			{
				fcntl(fildes, F_SETFL, oldFl);
				return false;
			}
			++endlinePos;
		}

		currPos = lseek(fildes, 0L, SEEK_CUR);
	}

	fcntl(fildes, F_SETFL, oldFl);
	return true;
}

void printIndentTable(IndentTable* table)
{
	printf("{ ");
	for(size_t i = 0; i < table->size; ++i)
	{
		printf("%ld, ", table->arr[i]);
	}
	printf(" }\n");
}

void destroyIndentTable(IndentTable* table)
{
	if(table->arr != NULL)
	{
		free(table->arr);
	}
}

bool printLine(int fildes, IndentTable* table, int lineNum)
{
	--lineNum;

	errno = 0;

	if(lineNum < 0 || lineNum >= table->size)
	{
		printf("No line with such number\n");
		return false;
	}

	off_t beginPos =  lineNum == 0
		  	? 0
			: table->arr[lineNum-1] + 1; //+1 to get next pos after '\n'
	off_t length = table->arr[lineNum] - beginPos  + 1; //+1 to include '\n'

	char* line = calloc(length + 1, sizeof(char));
	if(line == NULL)
	{
		perror("Cannot allocate memory to printLine buffer");
		return false;
	}


	lseek(fildes, beginPos, SEEK_SET);
	read(fildes, line, length);

	printf("%s", line);
	free(line);

	return true;
}

bool exitTimer(int timeout)
{
	static struct pollfd fds = {
			.fd = 0, //stdin
			.events = POLLIN,
			.revents = 0
	};

	return poll(&fds, 1, timeout) == 1;
}

bool scanAndCheckLine(off_t* lineNum)
{
	errno = 0;

	if(scanf("%ld", lineNum) == 0)
	{
		perror("Not off_t format");
		return false;
	}

	if(*lineNum == 0)
	{
		return false;
	}
	return true;
}

int main()
{
	errno = 0;

	int fildes = open("text.txt", O_RDONLY);

	if(fildes == -1)
	{
		perror("Cannot open file");
		return -1;
	}


	IndentTable table;
	initIndentTable(&table);

	fillIndentTable(&table, fildes);

	off_t lineNum;
	for(;;)
	{
		printf("Type line num to read within 5 sec. Type 0 to exit: ");
		fflush(stdout);

		if(exitTimer(TIMEOUT))
		{
			if(!scanAndCheckLine(&lineNum))
			{
				break;
			}
			printLine(fildes, &table, lineNum);
		} else
		{
			printf("Out of time. The program will exit\nWhole file:\n");
			for(int lineNum = 1; lineNum <= table.size; ++lineNum)
			{
				printLine(fildes, &table, lineNum);
			}
			break;
		}
	}

	close(fildes);
	destroyIndentTable(&table);

	return 0;
}
