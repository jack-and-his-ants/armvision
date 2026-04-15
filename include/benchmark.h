#ifndef ARMVISION_BENCHMARK_H
#define ARMVISION_BENCHMARK_H
#include "image.h"
#define NUM_TESTS 1000

void benchmark_grayscale(Image *img);
void benchmark_monochrome(Image *img);
void benchmark_negative(Image *img);


#endif //ARMVISION_BENCHMARK_H
