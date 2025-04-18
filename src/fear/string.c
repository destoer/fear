#include "fear/fear.h"

u32 fear_strlen(const char* str)
{
    u32 size = 0;

    while(str[size] != '\0')
    {
        size++;
    }

    return size;
}

u32 fear_is_digit(char digit)
{
    return digit >= '0' && digit <= '9';
}

struct String fear_make_str(const char* str) {
    struct String ans;
    ans.data = str;
    ans.size = fear_strlen(str);

    return ans;
}

struct String fear_make_str_slice(const char* str,u32 size) {
    struct String ans;
    ans.data = str;
    ans.size = size;

    return ans;
}


b8 fear_str_equal(const struct String v1, const struct String v2) {
    if(v1.size != v2.size) {
        return false;
    }

    for(u32 i = 0; i < v1.size; i++) {
        if(v1.data[i] != v2.data[i]) {
            return false;
        }
    }

    return true;
}

b8 fear_parse_int(const struct String str, s64* ans) {
    if(!str.size) {
        return true;
    }

    *ans = 0;

    u32 offset = 0;

    b8 negative = false;

    if(*str.data == '-') {
        negative = true;
        offset++;
    }

    for(u32 i = offset; i < str.size; i++) {
        const char cur = str.data[i];

        if(!fear_is_digit(cur)) {
            return true;
        }

        const int digit = cur - '0';

        *ans *= 10;
        *ans += digit;
    }

    if(negative) {
        *ans = -*ans;
    }

    return false;
}

void fear_destroy_heap_str(struct String* string)
{
    fear_free((char*)string->data);
    string->data = NULL;
    string->size = 0;
}

void fear_write_str(const struct String string)
{
    fear_write_chars(string.data);
}

struct String fear_vformat(const struct String fmt, va_list args)
{
    struct Array string_buffer = fear_make_array(sizeof(char));
    FEAR_UNUSED(fmt); FEAR_UNUSED(args); FEAR_UNUSED(string_buffer);

    return fear_str_from_buffer(&string_buffer);
}

struct String fear_format(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);

    const struct String string = fear_vformat(fear_make_str(fmt),args);
    va_end(args);   

    return string;
}

void fear_print(const char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);

    struct String string = fear_vformat(fear_make_str(fmt),args);

    va_end(args);

    fear_write_str(string);
    fear_destroy_heap_str(&string);
}

