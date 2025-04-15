#include <stdio.h>
#include <stdlib.h>

void fear_write_chars(const char* str)
{
    fprintf(stdout,"%s",str);
}

void* fear_alloc(size_t size)
{
    return malloc(size);
}

void* fear_realloc(void* ptr, size_t size)
{
    return realloc(ptr,size);
}

void fear_free(void* data)
{
    free(data);
}