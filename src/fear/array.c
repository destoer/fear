#include "fear/array.h"
#include "fear/logger.h"

#include <stdlib.h>
#include <string.h>

enum fear_error fear_resize_array(struct Array *array, u32 size) {
    void* buffer = realloc(array->data,size);

    if(!buffer) {
        FEAR_ERROR("Could not allocate array of size %zd\n",size);
        return FEAR_ERROR_OOM;
    }

    array->data = buffer;
    array->capacity = size;
    array->size = size;

    return FEAR_OK;
}

static enum fear_error fear_reserve_mem(struct Array* array, u32 bytes) {
    if(array->capacity - array->size < bytes) {
        const u32 new_capacity = (array->capacity + bytes) * 2;
        void* buffer = realloc(array->data,new_capacity);

        if(!buffer) {
            return FEAR_ERROR_OOM;
        }

        array->data = buffer;
        array->capacity = new_capacity;
    }

    return FEAR_OK;
}

enum fear_error fear_push_char(struct Array *array, char v) {
    const enum fear_error result = fear_reserve_mem(array,1);

    if(result == FEAR_OK) {
        array->data[array->size++] = v;
    }

    return result;
}

enum fear_error fear_push_mem(struct Array *array, const void* data, u32 size) {
    const enum fear_error result = fear_reserve_mem(array,size);

    if(result == FEAR_OK) {
        memcpy(&array->data[array->size],data,size);
        array->size += size;
    }

    return result;   
}

enum fear_error fear_push_var(struct Array *array, const void* data) {
    return fear_push_mem(array,data,array->elem_size);
}

struct Array fear_make_array(u32 elem_size) {
    struct Array array = {NULL,elem_size,0,0};

    return array;
}

void fear_destroy_array(struct Array* array) {
    free(array->data);
}

u32 fear_array_count(const struct Array* array) {
    return array->size / array->elem_size;
}

void fear_read_array(const struct Array* array, u32 index, void* data) {
    const u8* buffer = array->data;

    memcpy(data,&buffer[index * array->elem_size],array->elem_size);
}  