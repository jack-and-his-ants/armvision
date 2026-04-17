#ifndef ARMVISION_FILTERS_NEON_H
#define ARMVISION_FILTERS_NEON_H
#include "image.h"

void negative_neon(Image *img);
void grayscale_neon(Image *img);
void monochrome_neon(Image *img);

#endif //ARMVISION_FILTERS_NEON_H
