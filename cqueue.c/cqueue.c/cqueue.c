#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>
#include "atomic.h"

typedef uint64_t CQMsgType;
#define MSGTYPE_ALL		~0uL
#define MSGTYPE_NONE	 0uL

//typedef volatile uint32_t CQEntryState;
//#define QES_FREE	0x00000000u
//#define QES_ENQ		0x00000001u
//#define QES_READY	0x00000002u
//#define QES_DEQ		0x00000004u
//#define QES_DIRTY	0x00000008u

#define QSIZE		0x00000100u
#define QMASK		(QSIZE - 1)
#define MSGSIZE		0x00000100u

#define cq_isempty() (head == tail)
#define cq_isfull() (head == (tail - 1))

typedef union
{
	struct CQMessageHeader
	{
		uint64_t lock;
		uint64_t msgtype;
	};
	uint8_t data[MSGSIZE];
} CQMessage;

static CQMessage queue[QSIZE];
static uint32_t lead = 0;

void cq_enq(CQMessage msg)
{
	while (atomic_lock(queue[(++lead & QMASK)].lock))
	{

	}
}

int main(void)
{
	return 0;
}