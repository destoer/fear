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

#ifdef FEAR_STL_ENABLED
#include "stl.h"
#endif

#define FEAR_UNUSED(X) ((void)X)

struct FearContext
{
    struct Heap heap;
};

extern struct FearContext fear_context;