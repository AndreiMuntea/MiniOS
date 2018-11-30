#ifndef __HEAP_H__
#define __HEAP_H__

#define BASE_ADDRESS    0x1000000
#define PAGE_SIZE       4096
#define MAX_PAGES       64

#pragma pack(push, 1)
typedef struct _HEAP
{
    void* Chunks[MAX_PAGES];
}HEAP, *PHEAP;
#pragma pack(pop)

void 
HeapCreate(
    PHEAP Heap
);

void
HeapDestroy(
    PHEAP Heap
);

void*
HeapAlloc(
    PHEAP Heap
);

void
HeapFree(
    PHEAP Heap,
    void* Address
);

#endif //__HEAP_H__