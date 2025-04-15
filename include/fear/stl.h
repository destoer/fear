#include "array.h"
#include "error.h"

enum fear_error fear_read_bytes(const char* path, struct Array* array);
enum fear_error fear_read_str(const char* path, struct Array* array);