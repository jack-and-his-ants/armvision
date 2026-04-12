#include "filters.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

int clamp(const int v,const int min,const int max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}


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

void blurr(Image * bmp, int mask_size) {
    if (mask_size%2==0) {
        perror("Incorrect mask size: must be odd");
        return;
    }
    Image copy;
    copy.width = bmp->width;
    copy.height = bmp->height;
    copy.row_size = bmp->row_size;
    copy.data = (unsigned char*)malloc(copy.row_size * copy.height * sizeof(char));
    memcpy(copy.data, bmp->data, copy.row_size * copy.height * sizeof(char));

    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ){

            int result_b=0,result_g=0,result_r=0;

            for (int i = -mask_size/2 ; i <= mask_size/2 ; ++i ) {
                for (int j = -mask_size/2 ; j <= mask_size/2 ; ++j ) {

                    int ny = clamp(y + i, 0, bmp->height - 1);
                    int nx = clamp(x + j, 0, bmp->width - 1);

                    int idx = ny * bmp->row_size + nx * 3;

                    result_b += copy.data[idx];
                    result_g += copy.data[idx + 1];
                    result_r += copy.data[idx + 2];
                }
            }
            int z = 3 * x + y * bmp->row_size;
            int area = mask_size*mask_size;
            bmp->data[z] = result_b/area;
            bmp->data[z + 1] = result_g/area;
            bmp->data[z + 2] = result_r/area;
        }
    }

    free(copy.data);
}