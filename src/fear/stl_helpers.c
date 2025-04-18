#include "fear/stl.h"
#include "fear/logger.h"

#include <stdio.h>

static FILE* fear_read_file(const char* path, u32 *size) {
    FILE* fp = fopen(path,"r");

    if(!fp) {
        FEAR_ERROR("Could not read file: '%s'",path);
        return NULL;
    }

    fseek(fp,0,SEEK_END);
    *size = ftell(fp);
    rewind(fp);

    return fp;
} 

enum fear_error fear_read_bytes(const char* path, struct Array* array) {
    u32 size = 0;
    FILE* fp = fear_read_file(path,&size);

    const enum fear_error resize_res = fear_resize_array(array,size);

    if(resize_res) {
        FEAR_ERROR("Could not allocate memory for file: %s\n",path);
        fclose(fp);
        return resize_res;
    }

    fread(array->data,sizeof(u8),array->size,fp);
    fclose(fp);

    return FEAR_OK;
}

enum fear_error fear_read_str(const char* path, struct Array* array) {
    u32 size = 0;
    FILE* fp = fear_read_file(path,&size);

    if(!fp) {
        return FEAR_ERROR_FILE_READ;
    }

    const enum fear_error resize_res = fear_resize_array(array,size + 1);

    if(resize_res) {
        FEAR_ERROR("Could not allocate memory for file: %s\n",path);
        fclose(fp);
        return resize_res;
    }

    fread(array->data,sizeof(u8),array->size,fp);
    array->data[array->size - 1] = '\0';
    fclose(fp);

    return FEAR_OK;
}