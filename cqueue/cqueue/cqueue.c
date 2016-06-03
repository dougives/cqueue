#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "atomic.h"

#define STATE_FREE	0x00000000u
#define STATE_ENQ	0x00000001u
#define STATE_READY	0x00000002u

#define DATA_SIZE 0x100
#define QBUFFER_SIZE 0x100
#define QBUFFER_MASK (QBUFFER_SIZE - 1)

typedef struct
{
	uint32_t state;
	uint8_t data[DATA_SIZE];
} Message;

volatile static Message* qbuffer[QBUFFER_SIZE];


void cq_enq(Message* msg)
{
	volatile static size_t index = 0;  // yeehaw
	msg->state = STATE_ENQ;
	size_t i = index & QBUFFER_MASK;
	for (i; i = ++index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i]->state == STATE_FREE
			&& atomic_set32(&qbuffer[i]->state, STATE_ENQ)) // could do bitwise atomics
			break;
	}

	qbuffer[i] = msg;

	while(!atomic_set32(&qbuffer[i]->state, STATE_READY))
	{ }
	return;
}


int main(void)
{
	
	return 0;
}