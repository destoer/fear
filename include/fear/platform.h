#pragma once
#include "array.h"
struct File;

enum fear_file_mode
{
    FEAR_READ,
    FEAR_WRITE,
    FEAR_READ_WRITE,
};

enum fear_error fear_open_file(struct File** file, const struct String* name, enum fear_file_mode mode);
void fear_close_file(struct File** file);

enum fear_error fear_write_file(struct File* file, const struct ConstBuffer* buffer);

u32 fear_write_chars(const char* str);
void fear_panic(const char* str);


void* fear_acquire_memory(size_t size);

void fear_init_context();
void fear_destroy_context();

void fear_exit(int code);