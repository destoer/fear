#pragma once
#include "type.h"
#include "string.h"

// Note: This does not handle alignment, allocate only one size data on this.
struct Arena
{
    // How much have we used?
    u32 size; 

    // How much do we have total?
    u32 capacity;

    // Underyling memory.
    void* data;
};

struct Arena fear_make_arena(void* buffer, u32 size);
void* fear_arena_alloc(struct Arena* arena,u32 size); 

// 32 sets of doubling should be enough for any reasonable allocation.
#define FEAR_ARENA_ALLOC_SIZE 32

struct ArenaAllocator
{
    struct Arena arena[FEAR_ARENA_ALLOC_SIZE];
    u32 size;
};

struct ArenaAllocator fear_make_arena_allocator(void* buffer, u32 size);
void fear_destroy_arena_allocator(struct ArenaAllocator* alloc);
void* fear_arena_allocator_alloc(struct ArenaAllocator* allocator, u32 size);

enum fear_error fear_move_str_arena(struct String* str, struct ArenaAllocator* alloc);