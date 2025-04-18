#pragma once
#include "type.h"
#include "error.h"


struct Array {
    // Underlying data pointer.
    u8* data;

    // Size of a single data item.
    u32 elem_size;

    // Size of the array in bytes.
    u32 size;

    /// Capacity of the array in bytes.
    u32 capacity;
};

struct Array fear_make_array(u32 elem_size);
void fear_destroy_array(struct Array* array);
enum fear_error fear_resize_array(struct Array *array, u32 size);
enum fear_error fear_push_char(struct Array *array, char v);
enum fear_error fear_push_mem(struct Array *array, const void* data, u32 size);
enum fear_error fear_push_var(struct Array *array, const void* data);
b8 fear_pop_var(struct Array* array, void* data);
u32 fear_array_count(const struct Array* array);
void fear_read_array(const struct Array* array, u32 index, void* data);
struct String fear_str_from_buffer(struct Array* array);