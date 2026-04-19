#ifndef ARMVISION_FILTERS_NEON_H
#define ARMVISION_FILTERS_NEON_H
#include "image.h"

void negative_neon(Image *img);
void grayscale_neon(Image *img);
int monochrome_neon(Image *img,char channel);

#endif //ARMVISION_FILTERS_NEON_H
