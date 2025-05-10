#pragma once
#include "type.h"
#include "array.h"
#include "logger.h"
#include "string.h"
#include "arena.h"
#include "mem.h"
#include "platform.h"
#include "heap.h"
#include "math.h"

#define FEAR_UNUSED(X) ((void)X)

struct FearContext
{
    struct Heap heap;
    struct File* stdout;
    u8* brk_ptr;
};

extern struct FearContext fear_context;