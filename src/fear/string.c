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
    string->data = fear_free((char*)string->data);
    string->size = 0;
}

void fear_write_str(const struct String string) {
    fear_write_chars(string.data);
}

void fear_format_int(struct Array* string_buffer,s64 v, u64 base) {
    if(base > 16 || base == 0) {
        return;
    }

    if(v == 0) {
        fear_push_char(string_buffer,'0');
        return;
    }

    const u32 start_pos = string_buffer->size;

    const char* DIGIT_CHAR = "0123456789ABCDEF";
    u32 digits = 0;

    while(v != 0) {
        s64 digit = v % base;
        v = v / base;

        char character = DIGIT_CHAR[digit];
        fear_push_char(string_buffer,character);
        digits += 1;
    } 

    char* start = (char*)&string_buffer->data[start_pos];

    for(size_t i = 0; i < digits / 2; i++) {
        const char tmp = start[i];
        char* v2 = &start[digits - i - 1];
        start[i] = *v2;
        *v2 = tmp;
    }  
}

static void format_int_specifier(struct Array* string_buffer,char spec, s64 data)
{
    switch(spec) {
        case 'd':
        {
            fear_format_int(string_buffer,data,10);
            break;
        }

        case 'x':
        {
            fear_format_int(string_buffer,data,16);
            break;  
        }

        default:
        {
            break;
        }
    }
}

struct String fear_vformat(const struct String fmt, va_list args)
{
    struct Array string_buffer = fear_make_array(sizeof(char));
    // Having a buffer that is the size of the format string is a good lower limit on the size
    fear_reserve_mem(&string_buffer,fmt.size);

    for(u32 i = 0; i < fmt.size; i++) {

        if(fmt.data[i] != '%') {
            fear_push_char(&string_buffer,fmt.data[i]);
            continue;
        }

        // We are out of bounds
        if(i + 1 >= fmt.size) {
            break;
        }

        const char spec = fmt.data[i + 1];
        i++;

        switch(spec) {
            case 's':
            {
                const char* str = va_arg(args,const char*);
                while(*str != '\0')
                {
                    fear_push_char(&string_buffer,*str);
                    str++;
                }
                break;
            }

            case 'x':
            {
                const s32 data = va_arg(args,s32);
                fear_format_int(&string_buffer,data,16);
                break;
            }

            case 'd':
            {
                const s32 data = va_arg(args,s32);
                fear_format_int(&string_buffer,data,10);
                break;
            }

            case 'z':
            {
                // We are out of bounds
                if(i + 1 >= fmt.size)
                {
                    break;
                }


                const char spec_nested = fmt.data[i + 1];
                i++;

                const size_t data = va_arg(args,size_t);
                format_int_specifier(&string_buffer,spec_nested,data);
                break;
            }

            case 'l':
            {
                // We are out of bounds
                if(i + 1 >= fmt.size)
                {
                    break;
                }


                const char spec_nested = fmt.data[i + 1];
                i++;

                const s64 data = va_arg(args,s64);
                format_int_specifier(&string_buffer,spec_nested,data);
                break;
            }

            default:
            {
                fear_push_char(&string_buffer,spec);
                break;
            }
        }
    }

    return fear_str_from_buffer(&string_buffer);
}

struct String fear_format(const char *fmt, ...) {
    va_list args;
    va_start(args,fmt);

    const struct String string = fear_vformat(fear_make_str(fmt),args);
    va_end(args);   

    return string;
}

void fear_vprint(const char* fmt, va_list args) {
    struct String string = fear_vformat(fear_make_str(fmt),args);
    fear_write_str(string);
    fear_destroy_heap_str(&string);
}

void fear_print(const char *fmt, ...) {
    va_list args;
    va_start(args,fmt);
    
    fear_vprint(fmt,args);

    va_end(args);
}

