#pragma once
#include "type.h"
#include "error.h"

struct String {
    // NULL terminated string.
    const char *data;

    // Size of the string excluding the terminator.
    u32 size;
};

struct String fear_make_str(const char* str);
struct String fear_make_str_slice(const char* str,u32 size);
b8 fear_str_equal(const struct String v1, const struct String v2);

b8 fear_parse_int(const struct String str, s64* ans);

void fear_destroy_heap_str(struct String* string);
void fear_write_str(const struct String string);
struct String fear_format(const char* fmt, ...);
void fear_write_str(const struct String string);