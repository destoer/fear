#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "fear/string.h"

struct String fear_make_str(const char* str) {
    struct String ans;
    ans.data = str;
    ans.size = strlen(str);

    return ans;
}

struct String fear_make_str_slice(const char* str,u32 size) {
    struct String ans;
    ans.data = str;
    ans.size = size;

    return ans;
}


b8 fear_str_equal(const struct String* v1, const struct String* v2) {
    if(v1->size != v2->size) {
        return false;
    }

    for(u32 i = 0; i < v1->size; i++) {
        if(v1->data[i] != v2->data[i]) {
            return false;
        }
    }

    return true;
}

b8 fear_parse_int(const struct String* str, s64* ans) {
    if(!str->size) {
        return true;
    }

    *ans = 0;

    u32 offset = 0;

    b8 negative = false;

    if(*str->data == '-') {
        negative = true;
        offset++;
    }

    for(u32 i = offset; i < str->size; i++) {
        const char cur = str->data[i];

        if(!isdigit(cur)) {
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