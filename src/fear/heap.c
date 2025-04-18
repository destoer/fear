#include <fear/fear.h>


static bool pointer_in_bounds(void* ptr, size_t min, size_t max)
{
    const size_t value = (size_t)ptr;
    return value >= min && value <= max;
}

#include <stdio.h>
#include <stdlib.h>

static void validate_heap(struct Heap* heap)
{
    struct HeapNode* cur = heap->start;
    struct HeapNode* prev = NULL;

    while(cur)
    {
        if(cur->prev != prev)
        {
            printf("Prev does not match %p %p %p\n",cur,cur->prev,prev);
            exit(1);
        }

        if(!pointer_in_bounds(cur->next,heap->min_addr,heap->max_addr) && cur->next != NULL)
        {
            printf("Heap next out of bounds %p %zx %zx\n",cur->prev,heap->min_addr,heap->max_addr);
            exit(1);
        }

        if(!pointer_in_bounds(cur->prev,heap->min_addr,heap->max_addr) && prev != NULL)
        {
            printf("Heap prev out of boudnds %p %zx %zx\n",cur->next,heap->min_addr,heap->max_addr);
            exit(1);
        }

        if(cur->blocks == 0 || cur->blocks > heap->max_block)
        {
            printf("Heap blocks out of range: %p %zx %zx\n",cur,cur->blocks,heap->max_block);
            exit(1);
        }

        prev = cur;
        cur = cur->next;
    }
}

void* fear_heap_alloc(struct Heap* heap, u32 count, u32 size)
{
    validate_heap(heap);

    const u64 bytes = size * count;
    const u64 req_blocks = ((bytes + FEAR_HEAP_BLOCK_SIZE - 1) / FEAR_HEAP_BLOCK_SIZE) + 1;

    struct HeapNode* cur = heap->start;

    while(cur)
    {
        // Not free don't care
        if(!cur->free)
        {
            cur = cur->next;
            continue;
        }

        // Attempt to scan for a block with the right ammount of data
        if(cur->blocks == req_blocks)
        {
            // just remove it from the heap
            if(cur == heap->start)
            {
                heap->start = cur->next;
            }

            else
            {
                // This is not the start, we must have a prev
                cur->prev->next = cur->next;
                if(cur->next)
                {
                    cur->next->prev = cur->prev;
                }
            }

            cur->free = false;
            return cur + 1;
        }

        else if(cur->blocks > req_blocks)
        {
            struct HeapNode* old = cur;

            // fragment the block
            old->blocks -= req_blocks;
            cur += cur->blocks;
            
            cur->blocks = req_blocks;
            cur->free = false;

            cur->prev = old;
            cur->next = old->next;
            old->next = cur;
            
            return cur + 1;
        }

        cur = cur->next;
    }

    // Cannot acquire any more memory
    return NULL;
}

void fear_heap_free(struct Heap* heap, void* ptr)
{
    validate_heap(heap);

    // For now just ignore it
    FEAR_UNUSED(ptr);
}

u64 fear_umin(u64 v1, u64 v2)
{
    return v1 < v2? v1 : v2;
}

// This should be changed to try collapse blocks but it is easier if this just
// copies and frees
void* fear_heap_realloc(struct Heap* heap, void* old, u32 count, u32 size)
{
    validate_heap(heap);

    if(!old)
    {
        return fear_heap_alloc(heap,count,size);
    }

    void* new = fear_heap_alloc(heap,count,size);

    if(!new)
    {
        return NULL;
    }

    
    struct HeapNode* node = (struct HeapNode*)old;
    node -= 1;

    const u64 copy_size = fear_umin((node->blocks - 1) * FEAR_HEAP_BLOCK_SIZE,count * size);
    fear_memcpy(new,old,copy_size);

    fear_heap_free(heap,old);

    return new;
}

void fear_heap_init(struct Heap* heap, void* ptr, u32 size)
{
    if(size < FEAR_HEAP_BLOCK_SIZE)
    {
        return;
    }

    struct HeapNode* node = (struct HeapNode*)ptr;
    node->blocks = size / FEAR_HEAP_BLOCK_SIZE;
    node->next = NULL;
    node->prev = NULL;
    node->free = true;

    heap->start = node;

    heap->max_block = node->blocks;
    heap->max_addr = (size_t)&node[node->blocks];
    heap->min_addr = (size_t)node;

    validate_heap(heap);
}

void* fear_alloc(size_t count, size_t size)
{
    return fear_heap_alloc(&fear_context.heap,count,size);
}

void* fear_realloc(void* ptr, size_t count, size_t size)
{
    return fear_heap_realloc(&fear_context.heap,ptr,count,size);
}

void fear_free(void* data)
{
    fear_heap_free(&fear_context.heap,data);
}

void fear_init_context(void* heap_block, size_t size)
{
    fear_heap_init(&fear_context.heap,heap_block,size);
}