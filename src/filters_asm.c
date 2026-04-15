#include "image.h"
#include "filters_asm.h"

//extern void grayscale_asm(Image * bmp);
extern int monochrome_asm(Image * bmp, char channel);
