#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "atomic.h"

#define MESSAGE_SIZE 0x100
#define NODE_ENTRY_SIZE 0x100
//#define PAYLOAD_SIZE // sub size of header ...

typedef union
{
	uint64_t* lock;
	uint8_t payload[MESSAGE_SIZE];
} Message;

typedef struct
{
	struct Node* prev;
	struct Node* next;
	volatile size_t lasttaken;
	Message* entries[NODE_ENTRY_SIZE];
} Node;

typedef struct
{
	uint64_t* lock;
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

static void add_new_node()
{
	Node* node = (Node*)calloc(1, sizeof(Node));
	if (node == NULL)
		everything_broke();

	if (list.first == NULL && list.last == NULL)
	{
		while (true)
		{

		}
		list.first = node;
		list.last = node;
		return;
	}

	if (list.first == NULL
		|| list.last == NULL
		|| list.last->next != NULL)
		everything_broke();

	node->prev = list.last;
	node->entry = msg;
	list.last->next = node;
	list.last = node;
	return;
}

static Message* lock_msg()
{
	if (list.last->lasttaken >= NODE_ENTRY_SIZE)
		add_new_node();
}

bool cq_trydeq(Message* msg) // critical
{
	if (msg == NULL
		|| list.first == NULL)
		return false;

}

void cq_enq(const Message* msg)  // critical
{
	if (msg == NULL)
		return;

	Node* node = (Node*)calloc(1, sizeof(Node));
	if (node == NULL)
		everything_broke();

	if (list.first == NULL && list.last == NULL)
	{
		while (true)
		{

		}
		list.first = node;
		list.last = node;
		return;
	}

	if (list.first == NULL 
		|| list.last == NULL
		|| list.last->next != NULL)
		everything_broke();

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