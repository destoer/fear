#pragma once
struct HeapNode;

#define FEAR_HEAP_CANARY 0xdeadbeefcafebabe

struct HeapNode
{
    u64 canary_start;

    struct HeapNode* prev;
    struct HeapNode* next;
    // Inclusive of the node.
    size_t blocks;

    b8 free;

    u64 canary_end;
};

struct Heap
{
    struct HeapNode* start;
    size_t max_addr;
    size_t min_addr;
    size_t max_block;

    size_t in_use;
};

#define FEAR_HEAP_BLOCK_SIZE (sizeof(struct HeapNode))
#define DEFAULT_BLOCK_SIZE 1024 * FEAR_HEAP_BLOCK_SIZE


void* fear_alloc(size_t count, size_t size);
void* fear_calloc(size_t count, size_t size);
void* fear_realloc(void* ptr, size_t count, size_t size);
void* fear_free(void* data);

void fear_init_heap(struct Heap* heap);