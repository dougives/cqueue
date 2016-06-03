#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MESSAGE_SIZE 0x100
//#define PAYLOAD_SIZE // sub size of header ...

typedef union
{
	uint8_t payload[MESSAGE_SIZE];
} Message;

typedef struct
{
	uint64_t lock;
	struct Node* prev;
	struct Node* next;
	Message* entry;
} Node;

typedef struct
{
	Node* first;
	Node* last;
} List;

static List list;

static void everything_broke()
{
	printf("everything broke.\n\n");
	exit(EXIT_FAILURE);
}

static Message* get_empty_message()
{
	return (Message*)calloc(1, sizeof(Message));
}

void cq_enq(const Message* msg)  // critical
{
	if (msg == NULL)
		return;

	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL)
		everything_broke();

	if (list.first == NULL && list.last == NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		node->entry = msg;
		list.first = node;
		list.last = node;
		return;
	}

	if (list.first == NULL 
		|| list.last == NULL
		|| list.last->next != NULL)
		everything_broke();

	node->next = NULL;
	node->prev = list.last;
	node->entry = msg;
	list.last->next = node;
	list.last = node;
	return;
}

void cq_init()
{

	return;
}


int main(void)
{


	return 0;
}