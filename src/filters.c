#include "filters.h"

#include <stdio.h>

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

void monochrome(Image * bmp, char channel) {
    char mask[3] = {0,0,0};
    switch (channel) {
        case 'r':
            mask[2] = -1;
            break;
        case 'g':
            mask[1] = -1;
            break;
        case 'b':
            mask[0] = -1;
            break;
        default:
            perror("Unknown channel");
            return;
    }

    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int i = 3 * x + y * bmp->row_size;

            bmp->data[i] = bmp->data[i] & mask[0];
            bmp->data[i + 1] = bmp->data[i + 1] & mask[1];
            bmp->data[i + 2] = bmp->data[i + 2] & mask[2];
        }
    }
}

void negative(Image * bmp) {
    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int i = 3 * x + y * bmp->row_size;

            bmp->data[i] = -1 - bmp->data[i];
            bmp->data[i + 1] = -1 - bmp->data[i + 1];
            bmp->data[i + 2] = -1 - bmp->data[i + 2];
        }
    }
}