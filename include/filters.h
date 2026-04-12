#ifndef ARMVISION_FILTERS_H
#define ARMVISION_FILTERS_H
#include "image.h"

void grayscale(Image * image);
void monochrome(Image * image, char channel);
void negative(Image * image);
void blurr(Image * image, int mask_size);
void sobel(Image * image);

#endif //ARMVISION_FILTERS_H
