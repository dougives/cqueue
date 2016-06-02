#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MSGTYPE_ALL		~0uL
#define MSGTYPE_NONE	 0uL

typedef volatile uint32_t CQEntryState;
#define QES_FREE	0x00000000u
#define QES_ENQ		0x00000001u
#define QES_READY	0x00000002u
#define QES_DEQ		0x00000004u
#define QES_DIRTY	0x00000008u

#define BLOCKSIZE 0x10000u
#define TABLESIZE (BLOCKSIZE / sizeof(CQEntry))

typedef struct
{
	uint64_t msgtype;
	union
	{
		uint8_t data[0x100];
	};
} CQMessage;

typedef struct
{
	CQEntryState state;
	CQMessage msg;
} CQEntry;

typedef struct
{
	struct CQMemNode* prev;
	struct CQMemNode* next;
	size_t count;
	CQEntry* block;
} CQMemNode;

typedef struct
{
	CQMemNode* head;
	CQMemNode* tail;
} CQMemList;

typedef struct
{
	uint64_t msgtype;
	void* func;
} CQConsumer;



static CQMemList* memlist;

CQMemNode* cq_create_memnode()
{
	CQMemNode* node = malloc(sizeof(CQMemNode));
	node->block = malloc(sizeof(BLOCKSIZE));
	node->next = NULL;
	node->prev = NULL;
	node->count = 0;
	return node;
}

CQMemNode* cq_add_memnode()
{
	CQMemNode* node = memlist->tail;
	node->next = cq_create_memnode();
	node = node->next;
	node->prev = memlist->tail;
	memlist->tail = node;
}

void cq_enq(uint64_t msgtype, CQMessage msg)
{
	CQMemNode* node = memlist->tail;
	if (node->count >= TABLESIZE)
		node = cq_add_memnode();
	CQEntry entry;
	entry.state = QES_FREE;
	entry.msg = msg;
	node->block[node->count++] = entry;
}

void cq_init()
{
	memlist = malloc(sizeof(CQMemList));
	memlist->head = cq_create_memnode();
	memlist->tail = memlist->head;
}

int main(void)
{
	cq_init();
	return 0;
}