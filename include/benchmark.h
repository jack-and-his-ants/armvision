#ifndef ARMVISION_BENCHMARK_H
#define ARMVISION_BENCHMARK_H
#include "image.h"

void benchmark_grayscale(Image *img,int iters);
void benchmark_monochrome(Image *img, int iters);
void benchmark_negative(Image *img, int iters);



#endif //ARMVISION_BENCHMARK_H
