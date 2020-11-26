#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 16

typedef struct _Node
{
	char* str;
	struct _Node* nextNode;
} Node;

typedef struct _List
{
	Node* head;
} List;

List* initializeList()
{
	List* list = (List*) malloc(sizeof(List));
	if (list == NULL)
	{
		return NULL;
	}
	list->head = NULL;
	return list;
}

bool  pushToList(List* list, char* newStr)
{
	Node* node = (Node*) malloc(sizeof(Node));
	if(node == NULL)
	{
		printf("Cannot allocate memory for node\n");
		return false;
	}
	node->str = newStr;
	node->nextNode = list->head;
	list->head = node;
	return true;
}

void popList(List* list)
{
	if(list->head == NULL)
	{
		return;
	}
	Node* prev = list->head;
	char* prevStr = prev->str;
	if (prevStr != NULL)
	{
		free(prevStr);
	}
	list->head = list->head->nextNode;
	free(prev);
}

void deleteList(List* list)
{
	while(list->head != NULL)
	{
		popList(list);
	}
	free(list);
}

void printList(List* list)
{
	Node* currNode = list->head;
	while (currNode != NULL)
	{
		printf("%s", currNode->str);
		currNode = currNode->nextNode;
	}
}


int main() {
	char a[MAXSIZE];
	
	List* list = initializeList();

	printf("Input strings. Stop sign - '.' at the beginning\n");
	do
	{
		char* s = fgets(a, MAXSIZE, stdin);
		size_t len = strlen(s);
		char* newStr = (char*)malloc((len + 1) * sizeof(char));
		if (newStr == NULL)
		{
			break;
		}
		newStr[len] = '\0';
		memcpy(newStr, a, len);

		if(!pushToList(list, newStr))
		{
			free(newStr);
		}
	} while (a[0] != '.');

	popList(list);
	printList(list);
	deleteList(list);
	return 0;
}
