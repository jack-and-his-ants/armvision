#ifndef ARMVISION_FILTERS_H
#define ARMVISION_FILTERS_H
#include "image.h"

void grayscale(Image * image);
void negative(Image * image);
void blur(Image * image);
void sobel(Image * image);

#endif //ARMVISION_FILTERS_H
