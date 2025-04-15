#include "fear/mem.h"
#include <string.h>

void fear_zero_mem(void* data, u32 size) {
    memset(data,0,size);
}