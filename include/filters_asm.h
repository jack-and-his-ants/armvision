#ifndef ARMVISION_FILTERS_ASM_H
#define ARMVISION_FILTERS_ASM_H
#include "image.h"

void grayscale_asm(Image *bmp);
int monochrome_asm(Image *bmp, char channel);

#endif //ARMVISION_FILTERS_ASM_H
