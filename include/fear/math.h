#pragma once
#include <fear/type.h>

s64 fear_abs(s64 v1);

struct Vec2I {
    s32 x;
    s32 y;
};

s32 fear_min2_s32(s32 v1, s32 v2);
s32 fear_min3_s32(s32 v1, s32 v2, s32 v3);


s32 fear_max2_s32(s32 v1, s32 v2);
s32 fear_max3_s32(s32 v1, s32 v2, s32 v3);

u64 fear_min2_u64(u64 v1, u64 v2);

u64 fear_max2_u64(u64 v1, u64 v2);
u32 fear_max2_u32(u32 v1, u32 v2);

s32 fear_dot_product_2i(struct Vec2I v1, struct Vec2I v2);