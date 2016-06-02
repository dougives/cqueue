#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>
#include "atomic.h"

#define MSGTYPE_ALL		~0u
#define MSGTYPE_NONE	 0u

#define STATE_FREE	0x00000000u
#define STATE_ENQ	0x00000001u
#define STATE_READY	0x00000002u
#define STATE_DEQ	0x00000004u
//#define STATE_DIRTY	0x00000008u

#define QSIZE		0x00000100u
#define QMASK		(QSIZE - 1)
#define MSGSIZE		0x00000100u
#define HEADERSIZE	sizeof(struct CQMessageHeader)

#define cq_isempty() (head == tail)
#define cq_isfull() (head == (tail - 1))

typedef union
{
	struct CQMessageData
	{
		struct CQMessageHeader
		{
			volatile uint64_t lock;
			volatile uint32_t state;
			uint32_t type;
		};
		union CQMessagePayload
		{
			uint8_t payload[MSGSIZE - HEADERSIZE];
		};
	};
	uint8_t data[MSGSIZE];
} CQMessage;

static CQMessage queue[QSIZE];

CQMessage cq_deq()
{
	static uint32_t lead = 0;
	CQMessage msg;
	while (queue[++lead & QMASK].state != STATE_READY 
		|| !atomic_lock(&queue[lead].lock)) ;
	while (!atomic_set(&queue[lead].state, STATE_DEQ)) ;
	memcpy(&msg, &queue[lead], sizeof(CQMessage));
	// wish i could return here ... maybe gc?
	memcpy(queue[lead].data + HEADERSIZE, 0, sizeof(CQMessage));
	queue[lead].type = MSGTYPE_NONE;
	atomic_unlock(&queue[lead].lock);
	while (!atomic_set(&queue[lead].state, STATE_FREE));
	return msg;
}

void cq_enq(CQMessage msg)
{
	static uint32_t lead = 0;

	while (queue[++lead & QMASK].state != STATE_FREE //0
		|| !atomic_lock(&queue[lead].lock))
	while (!atomic_set(&queue[lead].state, STATE_ENQ))
	//queue[lead].state = STATE_ENQ;
	memcpy(&queue[lead], &msg, sizeof(CQMessage));
	atomic_unlock(&queue[lead].lock);
	while (!atomic_set(&queue[lead].state, STATE_READY));
	//queue[lead].state = STATE_READY;
}

int main(void)
{
	return 0;
}