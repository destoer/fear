#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void fear_log(const char* fmt,const char* prefix, const char* file, const char* function, int line,...) {
    va_list args;
    va_start(args,line);

    fprintf(stderr,"[%s %s:%d %s]: ",prefix,file,line,function);
    vfprintf(stderr,fmt,args);

    va_end(args);

    fprintf(stderr,"\n");
}