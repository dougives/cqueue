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
	uint8_t data[DATA_SIZE];
} Message;


volatile static Message* qbuffer[QBUFFER_SIZE];

bool cq_deq(Message* msg)
{
	volatile static size_t index = 0;
	size_t i = index & QBUFFER_MASK;
	for (i; i = ++index & QBUFFER_MASK; i++)
	{

	}

}

void cq_enq(Message* msg)
{
	volatile static size_t index = 0;  // yeehaw
	size_t i = index & QBUFFER_MASK;
	for (i; i = ++index & QBUFFER_MASK; i++)
	{
		if (qbuffer[i] == NULL
			&& atomic_set64(&qbuffer[i], msg))
			break;
	}
	return;
}


int main(void)
{
	
	return 0;
}