#include <fear/fear.h>


static bool pointer_in_bounds(void* ptr, size_t min, size_t max) {
    const size_t value = (size_t)ptr;
    return value >= min && value <= max;
}

static void validate_heap(struct Heap* heap) {
    struct HeapNode* cur = heap->start;
    struct HeapNode* prev = NULL;

    while(cur)
    {
        if(cur->canary_start != FEAR_HEAP_CANARY || cur->canary_end != FEAR_HEAP_CANARY) {
            fear_panic("Heap canary has been clobbered");
        }

        if(cur->prev != prev) {
            fear_panic("Heap prev does not match");
        }

        if(!pointer_in_bounds(cur->next,heap->min_addr,heap->max_addr) && cur->next != NULL) {
            fear_panic("Heap next is out of bounds");
        }

        if(!pointer_in_bounds(cur->prev,heap->min_addr,heap->max_addr) && prev != NULL) {
            fear_panic("Heap prev is out of bounds");
        }

        if(cur->blocks == 0 || cur->blocks > heap->max_block) {
            fear_panic("Heap blocks out of range");
        }

        prev = cur;
        cur = cur->next;
    }
}

struct HeapNode* find_first_fit(struct Heap* heap, u32 req_blocks)
{
    struct HeapNode* cur = heap->start;

    while(cur) {   

        if(cur->blocks >= req_blocks && cur->free) {
            return cur;
        }

        cur = cur->next;
    }

    return NULL;
}

void fear_init_heap(struct Heap* heap) {
    heap->max_addr = (size_t)(0);
    heap->min_addr = (size_t)(-1);
    heap->in_use = 0;
    heap->start = NULL;
}

static void fear_heap_add_block(struct Heap* heap, struct HeapNode* node)
{
    struct HeapNode* cur = heap->start;
    
    if(!cur)
    {
        heap->start = node;
        return;
    }

    while(cur->next)
    {
        cur = cur->next;
    }

    cur->next = node;
    node->prev = cur;
}

void fear_heap_acquire(struct Heap* heap, size_t size) {
    if(size < FEAR_HEAP_BLOCK_SIZE) {
        return;
    }

    void* ptr = fear_acquire_memory(size);

    if(!ptr)
    {
        return;
    }

    fear_zero_mem(ptr,size);

    struct HeapNode* node = (struct HeapNode*)ptr;
    node->blocks = size / FEAR_HEAP_BLOCK_SIZE;
    node->next = NULL;
    node->prev = NULL;
    node->free = true;
    node->canary_start = FEAR_HEAP_CANARY;
    node->canary_end = FEAR_HEAP_CANARY;

    heap->max_addr = fear_max2_u64(heap->max_addr,(u64)&node[node->blocks]);
    heap->min_addr = fear_min2_u64(heap->min_addr,(size_t)node);
    heap->max_block += node->blocks;

    fear_heap_add_block(heap,node);

    validate_heap(heap);
}


void* fear_heap_alloc(struct Heap* heap, u32 count, u32 size)
{
    validate_heap(heap);

    const u64 bytes = size * count;
    const u64 req_blocks = ((bytes + FEAR_HEAP_BLOCK_SIZE - 1) / FEAR_HEAP_BLOCK_SIZE) + 1;

    struct HeapNode* cur = find_first_fit(heap,req_blocks);

    if(!cur) {
        // Attempt to grab the memory we are missing.
        fear_heap_acquire(heap,fear_max2_u64(DEFAULT_BLOCK_SIZE,bytes * 2));
        cur = find_first_fit(heap,req_blocks);

        if(!cur)
        {
            fear_write_chars("Out of memory!");
            return NULL;
        }
    }

    if(cur->blocks == req_blocks) {
        cur->free = false;
        heap->in_use += req_blocks * FEAR_HEAP_BLOCK_SIZE;
        return cur + 1;
    } 
    
    else if(cur->blocks > req_blocks) {
        struct HeapNode* old = cur;

        const size_t remain = old->blocks - req_blocks; 

        // fragment the block
        old->blocks = req_blocks;
        old->free = false;

        cur += old->blocks;
        
        cur->blocks = remain;
        cur->free = true;
        // Point the new block at its neighbours 
        cur->prev = old;
        cur->next = old->next;
        cur->canary_start = FEAR_HEAP_CANARY;
        cur->canary_end = FEAR_HEAP_CANARY;

        // Repoint the surrounding blocks
        if(cur->next) {
            cur->next->prev = cur;
        }

        old->next = cur;
        
        heap->in_use += req_blocks * FEAR_HEAP_BLOCK_SIZE;

        return old + 1;
    }

    return NULL;
}

void fear_heap_free(struct Heap* heap, void* ptr)
{
    if(!ptr)
    {
        return;
    }

    validate_heap(heap);

    struct HeapNode* node = (struct HeapNode*)ptr;
    node -= 1;

    // Mark as free
    node->free = true;
    heap->in_use -= node->blocks * FEAR_HEAP_BLOCK_SIZE;

    {
        struct HeapNode* next = node->next;

        // Attempt to collapse upper
        if(next && next->free && (node + node->blocks) == next) {
            node->blocks += next->blocks;
            
            // Link over next
            node->next = next->next;

            // Link back prev of new next
            if(node->next) {
                node->next->prev = node;
            }
        }
    }

    {
        struct HeapNode* prev = node->prev;

        // Attempt to collapse before
        if(prev && prev->free && (prev + prev->blocks) == node) {
            prev->blocks += node->blocks;

            // Link over node
            prev->next = node->next;

            // Link back prev of node next
            if(prev->next) {
                prev->next->prev = prev;
            }
        }
    }
}

// This should be changed to try collapse blocks but it is easier if this just
// copies and frees
void* fear_heap_realloc(struct Heap* heap, void* old, u32 count, u32 size) {
    validate_heap(heap);

    if(!old) {
        return fear_heap_alloc(heap,count,size);
    }

    void* new = fear_heap_alloc(heap,count,size);

    if(!new) {
        return NULL;
    }

    
    struct HeapNode* node = (struct HeapNode*)old;
    node -= 1;

    const u64 copy_size = fear_min2_u64((node->blocks - 1) * FEAR_HEAP_BLOCK_SIZE,count * size);
    fear_memcpy(new,old,copy_size);
    fear_heap_free(heap,old);

    validate_heap(heap);
    return new;
}

void* fear_alloc(size_t count, size_t size) {
    return fear_heap_alloc(&fear_context.heap,count,size);
}

void* fear_realloc(void* ptr, size_t count, size_t size) {
    return fear_heap_realloc(&fear_context.heap,ptr,count,size);
}

void* fear_free(void* data) {
    fear_heap_free(&fear_context.heap,data);
    return NULL;
}