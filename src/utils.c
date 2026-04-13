#include "utils.h"

static inline int32_t clampC(int32_t v, int32_t min, int32_t max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

extern int32_t clamp_asm(int32_t, int32_t, int32_t);

clamp_func_t clamp_impl = clampC;

int32_t clamp(int32_t v, int32_t min, int32_t max) {
    return clamp_impl(v, min, max);
}