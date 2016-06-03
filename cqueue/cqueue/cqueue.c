#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "atomic.h"

#define DATA_SIZE 0x100
#define QBUFFER_SIZE 0x100
#define QBUFFER_MASK (QBUFFER_SIZE - 1)

typedef struct
{
	uint32_t lock;
	uint8_t data[DATA_SIZE];
} Message;


volatile static Message* qbuffer[QBUFFER_SIZE];

Message* cq_deq()
{
	volatile static size_t index = 0;
	Message* msg;
	size_t i = index & QBUFFER_MASK;
	for (i; i = ++index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i] != NULL 
			&& qbuffer[i]->lock == ATOMIC_UNLOCKED
			&& atomic_lock(&qbuffer[i]->lock))
			break;
	}
	msg = qbuffer[i];
	
	while (!atomic_set64(&qbuffer[i], NULL))
	{ }
}

void cq_enq(Message* msg)
{
	volatile static size_t index = 0;  // yeehaw

	msg->lock = ATOMIC_UNLOCKED;
	for (size_t i = index & QBUFFER_MASK; i = ++index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i] == NULL
			&& atomic_set64(&qbuffer[i], msg))
			break;
	}
	return;
}


int main(void)
{
	Message msg;
	memset(&msg, 0, sizeof(Message));
	Message* msgptr = &msg;
	cq_enq(msgptr);
	msgptr = cq_deq();
	return 0;
}