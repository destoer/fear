#include "fear/arena.h"
#include "fear/mem.h"
#include "fear/logger.h"
#include "fear/platform.h"

struct Arena fear_make_arena(void* buffer, u32 size) {
    struct Arena arena;

    arena.data = buffer;

    if(!arena.data) {
        size = 0;
    }

    arena.capacity = size;
    arena.size = 0;

    return arena;
}

void fear_destroy_arena(struct Arena* arena) {
    fear_free(arena->data);
}

void* fear_arena_alloc(struct Arena* arena,u32 size) {
    if(arena->capacity - arena->size < size) {
        return NULL;
    }

    u8* data = (u8*)arena->data;
    u8* block  = &data[arena->size];
    arena->size += size;

    return block;
}

struct ArenaAllocator fear_make_arena_allocator(void* buffer, u32 size) {
    struct ArenaAllocator alloc;
    fear_zero_mem(&alloc,sizeof(alloc));

    alloc.arena[0] = fear_make_arena(buffer,size);
    alloc.size = 1;

    return alloc;
}

void fear_destroy_arena_allocator(struct ArenaAllocator* alloc) {
    for(u32 i = 0; i < alloc->size; i++) {
        fear_destroy_arena(&alloc->arena[i]);
    }
}


void* fear_arena_allocator_alloc(struct ArenaAllocator* allocator, u32 size) {
    {
        struct Arena* arena = &allocator->arena[allocator->size - 1];

        if(arena->size + size >= arena->capacity) {
            const u32 new_arena_size = arena->capacity * 2;
            void* buffer = fear_alloc(arena->capacity * 2);

            if(!buffer) {
                FEAR_ERROR("Could not allocate new arena for allocation: %zd",size);
                return NULL;
            }

            allocator->arena[allocator->size] = fear_make_arena(buffer,new_arena_size);
            allocator->size += 1;
        }
    }

    struct Arena* arena = &allocator->arena[allocator->size - 1];
    return fear_arena_alloc(arena,size);
}

enum fear_error fear_move_str_arena(struct String* str, struct ArenaAllocator* alloc) {
    char* data = fear_arena_allocator_alloc(alloc,str->size + 1);

    if(!data) {
        FEAR_ERROR("Out of memory to move string to arena of size: %zd",str->size);
        return FEAR_ERROR_OOM;
    }

    fear_memcpy(data,str->data,str->size);
    data[str->size] = '\0';
    str->data = data;

    return FEAR_OK;
}