#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>
#include <stdio.h>
#include <windows.h>
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


typedef union
{
	struct CQMessageData
	{
		struct CQMessageHeader
		{
			volatile uint64_t lock;
			volatile uint32_t state;
			uint32_t rsvd;
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
	static uint32_t lead = 0; // yeehaw
	CQMessage msg;
	CQMessage* grabbed;
	while ((grabbed = &queue[++lead & QMASK])->state != STATE_READY
		|| !atomic_lock(&grabbed->lock))
	{
		printf("卡");
	}
	;
	while (!atomic_set(&grabbed->state, STATE_DEQ)) ;
	memcpy(&msg, grabbed, sizeof(CQMessage));
	// wish i could return here ... maybe gc?
	//memcpy(queue[lead].payload, 0, sizeof(union CQMessagePayload));
	grabbed->rsvd = MSGTYPE_NONE;
	atomic_unlock(&grabbed->lock);
	while (!atomic_set(&grabbed->state, STATE_FREE));
	return msg;
}

void cq_enq(const CQMessage* msg)
{
	static uint32_t lead = 0;
	CQMessage* grabbed;
	while ((grabbed = &queue[++lead & QMASK])->state != STATE_FREE //0
		|| !atomic_lock(&grabbed->lock)) ;
	while (!atomic_set(&grabbed->state, STATE_ENQ)) ;
	//queue[lead].state = STATE_ENQ;
	memcpy(grabbed->payload, msg->payload, sizeof(union CQMessagePayload));
	atomic_unlock(&grabbed->lock);
	while (!atomic_set(&grabbed->state, STATE_READY)) ;
	//queue[lead].state = STATE_READY;
}

void cq_test_enqer()
{
	static CQMessage msg;
	static char i;
	while (true)
	{
		memset(&msg, 0, sizeof(CQMessage));
		msg.payload[0] = (i++ & 0x0f) + 0x61;
		cq_enq(&msg);
	}
}

void cq_test_deqer()
{
	while (true)
	{
		printf("%c", cq_deq().payload[0]);
	}
}

int main(void)
{
	LPDWORD threadid = NULL;
	CreateThread(NULL, 0, cq_test_enqer, NULL, 0, threadid);
	CreateThread(NULL, 0, cq_test_deqer, NULL, 0, threadid);
	while (true)
	{
		Sleep(100);
	}
	return 0;
}