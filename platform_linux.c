#include <fear/fear.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


void fear_exit(int code) {
    exit(code);
}

struct File
{
    int fd;
};

u32 fear_write_chars(const char* str) {
    if(!fear_context.stdout)
    {
        fear_exit(0x66);
    }

    const u32 size = fear_strlen(str);
    fear_write_file(fear_context.stdout,&(struct ConstBuffer){.data = str,.size = size});

    return size;
}

void fear_panic(const char* str) {
    fear_write_chars(str);
    fear_write_chars("\n");
    fear_exit(1);
}

void fear_init_context() {
    fear_context.stdout = NULL;

    fear_context.brk_ptr = sbrk(0);
    fear_init_heap(&fear_context.heap);

    const struct String name = fear_make_str("/dev/stdout");

    if(fear_open_file(&fear_context.stdout,&name,FEAR_READ) != FEAR_OK)
    {
        fear_exit(0x55);
    }
}

void fear_destroy_context() {
    fear_close_file(&fear_context.stdout);
}

enum fear_error fear_open_file(struct File** file, const struct String* name, enum fear_file_mode mode) {
    int linux_mode = 0;

    switch(mode)
    {
        case FEAR_READ:
        {
            linux_mode = O_RDONLY;
            break;
        }

        case FEAR_WRITE:
        {
            linux_mode = O_WRONLY;
            break;
        }

        case FEAR_READ_WRITE:
        {
            linux_mode = O_RDWR;
            break;
        }
    }

    const int fd = open(name->data,0,linux_mode);

    if(fd == -1)
    {
        return FEAR_ERROR_FILE_OPEN;
    }

    struct File* opened_file = fear_alloc(1,sizeof(struct File));

    if(!opened_file)
    {
        return FEAR_ERROR_OOM;
    }

    *file = opened_file;

    return FEAR_OK;
}

enum fear_error fear_write_file(struct File* file, const struct ConstBuffer* buffer) {

    const int rc = write(file->fd,buffer->data,buffer->size);

    return rc < 0? FEAR_OK : FEAR_ERROR_FILE_WRITE;
}

void fear_close_file(struct File** file)
{
    close((*file)->fd);
    file = fear_free(file);
}

void* fear_acquire_memory(size_t size) {
    u8* old = fear_context.brk_ptr;

    fear_context.brk_ptr = sbrk(size);

    return old;
}