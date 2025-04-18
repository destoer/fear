#pragma once
struct HeapNode;

struct HeapNode
{
    struct HeapNode* prev;
    struct HeapNode* next;
    // Inclusive of the node.
    size_t blocks;

    b8 free;
};

struct Heap
{
    struct HeapNode* start;
    size_t max_addr;
    size_t min_addr;
    size_t max_block;
};


#define FEAR_HEAP_BLOCK_SIZE (sizeof(struct HeapNode))


void* fear_alloc(size_t count, size_t size);
void* fear_realloc(void* ptr, size_t count, size_t size);
void fear_free(void* data);
void fear_init_context(void* heap_block, size_t size);
