#pragma once
#include "type.h"

void fear_zero_mem(void* data, u32 size);
void fear_memset(void* dst, u8 value, u32 size);
void fear_memcpy(void* dst, const void* src, u32 size);