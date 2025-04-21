#include <fear/math.h>

s64 fear_abs(s64 v1) {
    return v1 < 0? -v1 : v1;
}

s32 fear_min2_s32(s32 v1, s32 v2) {
    return v1 < v2? v1 : v2;
}

s32 fear_min3_s32(s32 v1, s32 v2, s32 v3) {
    s32 min = fear_min2_s32(v1,v2);
    min = fear_min2_s32(min,v3);

    return min;
}

s32 fear_max2_s32(s32 v1, s32 v2) {
    return v1 > v2? v1 : v2;
}

s32 fear_max3_s32(s32 v1, s32 v2, s32 v3) {
    s32 max = fear_max2_s32(v1,v2);
    max = fear_max2_s32(max,v3);

    return max;
}

s32 fear_dot_product_2i(struct Vec2I v1, struct Vec2I v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}