#include <stdio.h>
#include <stdlib.h>

void fear_write_chars(const char* str) {
    fprintf(stdout,"%s",str);
}

void fear_panic(const char* str) {
    fear_write_chars(str);
    fear_write_chars("\n");
    exit(1);
}