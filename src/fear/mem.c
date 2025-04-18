#include "fear/mem.h"

void fear_memset(void* dst, u8 value, u32 size) {
    u8* buffer = (u8*)dst;

    for(u32 i = 0; i < size; i++) {
        buffer[i] = value;
    }
}

void fear_memcpy(void* dst, const void* src, u32 size) {
    u8* dst_buffer = (u8*)dst;
    u8* src_buffer = (u8*)src;

    for(u32 i = 0; i < size; i++) {
        dst_buffer[i] = src_buffer[i];
    }
}

void fear_zero_mem(void* dst, u32 size) {
    fear_memset(dst,0,size);
}