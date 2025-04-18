#include <fear/string.h>

void fear_log(const char* fmt,const char* prefix, const char* file, const char* function, int line,...) {
    va_list args;
    va_start(args,line);

    fear_print("[%s %s:%d %s]: ",prefix,file,line,function);
    fear_vprint(fmt,args);

    va_end(args);

    fear_write_chars("\n");
}