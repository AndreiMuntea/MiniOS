#include "heap.h"
#include "utils.h"
#include "asm_definitions.h"
#include "definitions.h"

void 
HeapCreate(
    PHEAP Heap
)
{
    MemInit();
    UtilsZeroMemory((char*)(Heap->Chunks), sizeof(Heap->Chunks));
}

void
HeapDestroy(
    PHEAP Heap
)
{
    for(BYTE i = 0; i < MAX_PAGES; ++i)
    {
        if(Heap->Chunks[i] != NULL)
        {
            MemFreePage(i);
        }
    }
    
    UtilsZeroMemory((char*)(Heap->Chunks), sizeof(Heap->Chunks));
    MemUninit();
}

void*
HeapAlloc(
    PHEAP Heap
)
{
    BYTE pageIndex = (BYTE)MemAllocPage();
    
    if(pageIndex == -1)
    {
        return NULL;
    }

    Heap->Chunks[pageIndex] = (void*)(BASE_ADDRESS + PAGE_SIZE * (QWORD)(pageIndex));
    return Heap->Chunks[pageIndex];
}

void
HeapFree(
    PHEAP Heap,
    void* Address
)
{
    QWORD pageIndex = (QWORD)Address - BASE_ADDRESS;

    if ((QWORD)Address < BASE_ADDRESS)
    {
        return;
    }

    if (pageIndex % PAGE_SIZE != 0)
    {
        return;
    }

    pageIndex /= PAGE_SIZE;

    MemFreePage((BYTE)(pageIndex));

    Heap->Chunks[pageIndex] = NULL;
}