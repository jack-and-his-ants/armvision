#include "filters.h"
#include "image.h"

void grayscale(Image * bmp) {
    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int i = 3 * x + y * bmp->row_size;

            unsigned char b = bmp->data[i];
            unsigned char g = bmp->data[i + 1];
            unsigned char r = bmp->data[i + 2];

            unsigned int gray = ( b * 29 + g * 150 + r * 77 ) >> 8;

            bmp->data[i] = gray;
            bmp->data[i + 1] = gray;
            bmp->data[i + 2] = gray;
        }
    }
}