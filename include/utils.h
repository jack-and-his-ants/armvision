#ifndef ARMVISION_UTILS_H
#define ARMVISION_UTILS_H

#include <stdint.h>

int32_t clamp(int32_t v, int32_t min, int32_t max);

typedef int32_t (*clamp_func_t)(int32_t, int32_t, int32_t);
extern clamp_func_t clamp_impl;

#endif