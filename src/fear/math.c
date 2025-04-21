#include <fear/math.h>

s64 fear_abs(s64 v1) {
    return v1 < 0? -v1 : v1;
}