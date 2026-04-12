#ifndef ARMVISION_BMP_H
#define ARMVISION_BMP_H
#include "image.h"

Image * load_bmp(const char * filename);

int save_bmp(Image * bmp, const char * filename);

void free_bmp(Image * bmp);

#endif //ARMVISION_BMP_H
