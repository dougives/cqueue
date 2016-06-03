#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "atomic.h"

#define DATA_SIZE 0x100
#define QBUFFER_SIZE 0x40
#define QBUFFER_MASK (QBUFFER_SIZE - 1)

typedef struct
{
	uint64_t lock;
	uint8_t data[DATA_SIZE];
} Message;


volatile static Message* qbuffer[QBUFFER_SIZE];

Message* cq_deq()
{
	volatile static size_t index = 0;
	Message* msg = NULL;
	size_t i = ++index & QBUFFER_MASK;
	for (i; i = index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i] != NULL 
			&& qbuffer[i]->lock == ATOMIC_UNLOCKED
			&& atomic_lock(&qbuffer[i]->lock))
			break;
	}
	msg = qbuffer[i];
	
	while (!atomic_set64((uint64_t*)&qbuffer[i], (uint64_t)NULL))
	{ }

	return msg;
}

void cq_enq(Message* msg)
{
	volatile static size_t index = 0;  // yeehaw

	msg->lock = ATOMIC_UNLOCKED;
	for (size_t i = ++index & QBUFFER_MASK; i = index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i] == NULL
			&& atomic_set64((uint64_t*)&qbuffer[i], (uint64_t)msg))
			break;
	}
	return;
}

volatile static long tp = 0;
void test_enq()
{
	while (true)
	{
		Message msg;
		Message* msgptr = &msg;
		memset(msgptr, 0, sizeof(Message));
		cq_enq(msgptr);
		//Sleep(1);
	}
}

void test_deq()
{
	Message* msgptr = NULL;
	while (true)
	{
		msgptr = cq_deq();
		tp++;
		//Sleep(1);
	}
}

void test_tp()
{
	while (true)
	{
		tp = 0;
		Sleep(1000);
		printf("%d\n", tp);
	}
}


int main(void)
{
	Message msg;
	Message* msgptr = &msg;
	memset(msgptr, 0, sizeof(Message));
	cq_enq(msgptr);
	msgptr = cq_deq();

	LPDWORD threadid = NULL;
	CreateThread(NULL, 0, test_tp, NULL, 0, threadid);
	CreateThread(NULL, 0, test_deq, NULL, 0, threadid);
	CreateThread(NULL, 0, test_enq, NULL, 0, threadid);	
	CreateThread(NULL, 0, test_deq, NULL, 0, threadid);
	CreateThread(NULL, 0, test_enq, NULL, 0, threadid);	
	CreateThread(NULL, 0, test_deq, NULL, 0, threadid);
	CreateThread(NULL, 0, test_enq, NULL, 0, threadid);

	while (true)
		Sleep(100);

	return 0;
}