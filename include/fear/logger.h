
#ifdef FEAR_LOGGING

void fear_log(const char* fmt,const char* prefix, const char* file, const char* function, int line,...);

#define FEAR_ERROR(FMT,...) (fear_log((FMT),"ERROR",__FILE__, __func__, __LINE__,##__VA_ARGS__))
#define FEAR_DEBUG(FMT,...) (fear_log((FMT),"DEBUG",__FILE__, __func__, __LINE__,##__VA_ARGS__))

#define FEAR_PANIC(FMT,...) {\
    fear_log((FMT),"PANIC",__FILE__, __func__, __LINE__,##__VA_ARGS__);\
    exit(1);\
}

#else 

#define FEAR_ERROR(FMT,...)
#define FEAR_DEBUG(FMT,...)
#define FEAR_PANIC(FMT,...) (exit(1))

#endif