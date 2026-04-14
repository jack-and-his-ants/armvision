#include "filters.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


static void generate_kernel(float *kernel, int size, float sigma) {
    int half = size / 2;
    float sum = 0.0f;

    for (int i = 0; i < size; i++) {
        int x = i - half;
        kernel[i] = expf(-(x * x) / (2 * sigma * sigma));
        sum += kernel[i];
    }

    for (int i = 0; i < size; i++) {
        kernel[i] /= sum;
    }
}

void grayscale(Image * bmp) {
    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int32_t i = 3 * x + y * bmp->row_size;

            uint8_t b = bmp->data[i];
            uint8_t g = bmp->data[i + 1];
            uint8_t r = bmp->data[i + 2];

            uint32_t gray = ( b * 29 + g * 150 + r * 77 ) >> 8;

            bmp->data[i] = gray;
            bmp->data[i + 1] = gray;
            bmp->data[i + 2] = gray;
        }
    }
}

void monochrome(Image * bmp, char channel) {
    if ( channel != 'b' && channel != 'g' && channel != 'r' ) {
            perror("Incorrect channel");
            return;
    }

    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int32_t i = 3 * x + y * bmp->row_size;

            bmp->data[i] = (channel == 'b' ? bmp->data[i] : 0);
            bmp->data[i + 1] = (channel == 'g' ? bmp->data[i+1] : 0);
            bmp->data[i + 2] = (channel == 'r' ? bmp->data[i+2] : 0);
        }
    }
}

void negative(Image * bmp) {
    for ( int y = 0; y < bmp->height; ++y ) {
        for ( int x = 0; x < bmp->width; ++x ) {
            int32_t i = 3 * x + y * bmp->row_size;

            bmp->data[i] = 255 - bmp->data[i];
            bmp->data[i + 1] = 255 - bmp->data[i + 1];
            bmp->data[i + 2] = 255 - bmp->data[i + 2];
        }
    }
}

void blur(Image * bmp, int mask_size) {
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

            int32_t result_b=0,result_g=0,result_r=0;

            for (int i = -mask_size/2 ; i <= mask_size/2 ; ++i ) {
                for (int j = -mask_size/2 ; j <= mask_size/2 ; ++j ) {

                    int32_t ny = clamp(y + i, 0, bmp->height - 1);
                    int32_t nx = clamp(x + j, 0, bmp->width - 1);

                    int32_t idx = ny * bmp->row_size + nx * 3;

                    result_b += copy.data[idx];
                    result_g += copy.data[idx + 1];
                    result_r += copy.data[idx + 2];
                }
            }
            int32_t z = 3 * x + y * bmp->row_size;
            int area = mask_size*mask_size;
            bmp->data[z] = result_b/area;
            bmp->data[z + 1] = result_g/area;
            bmp->data[z + 2] = result_r/area;
        }
    }

    free(copy.data);
}

void gaussian_blur(Image *bmp, int size) {

    if (size % 2 == 0) {
        perror("mask must be odd");
        return;
    }

    int half = size / 2;
    float sigma = size / 6.0f;

    // 1D kernel
    float *kernel = malloc(size * sizeof(float));
    generate_kernel(kernel, size, sigma);

    // Buffer
    Image temp = *bmp;
    temp.data = malloc(temp.row_size * temp.height);

    // Horizontal pass
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {

            float sum_b = 0, sum_g = 0, sum_r = 0;

            for (int k = -half; k <= half; k++) {
                int nx = clamp(x + k, 0, bmp->width - 1);
                int idx = y * bmp->row_size + nx * 3;

                float w = kernel[k + half];

                sum_b += bmp->data[idx] * w;
                sum_g += bmp->data[idx + 1] * w;
                sum_r += bmp->data[idx + 2] * w;
            }

            int z = y * bmp->row_size + x * 3;
            temp.data[z]     = (uint8_t)sum_b;
            temp.data[z + 1] = (uint8_t)sum_g;
            temp.data[z + 2] = (uint8_t)sum_r;
        }
    }
    // Vertical pass
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {

            float sum_b = 0, sum_g = 0, sum_r = 0;

            for (int k = -half; k <= half; k++) {
                int ny = clamp(y + k, 0, bmp->height - 1);
                int idx = ny * bmp->row_size + x * 3;

                float w = kernel[k + half];

                sum_b += temp.data[idx] * w;
                sum_g += temp.data[idx + 1] * w;
                sum_r += temp.data[idx + 2] * w;
            }

            int z = y * bmp->row_size + x * 3;
            bmp->data[z]     = (uint8_t)sum_b;
            bmp->data[z + 1] = (uint8_t)sum_g;
            bmp->data[z + 2] = (uint8_t)sum_r;
        }
    }

    free(kernel);
    free(temp.data);
}


void sobel(Image *bmp) {

    grayscale(bmp);

    Image copy = *bmp;
    copy.data = malloc(copy.row_size * copy.height);
    memcpy(copy.data, bmp->data, copy.row_size * copy.height);

    int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {

            int sum_x = 0;
            int sum_y = 0;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {

                    int ny = clamp(y + i, 0, bmp->height - 1);
                    int nx = clamp(x + j, 0, bmp->width - 1);

                    int idx = ny * bmp->row_size + nx * 3;

                    uint8_t val = copy.data[idx]; // grayscale

                    sum_x += val * gx[i + 1][j + 1];
                    sum_y += val * gy[i + 1][j + 1];
                }
            }
            // magnitude
            int mag = (int)sqrt(sum_x * sum_x + sum_y * sum_y);

            mag = mag > 255 ? 255 : mag;

            int z = y * bmp->row_size + x * 3;

            bmp->data[z]     = mag;
            bmp->data[z + 1] = mag;
            bmp->data[z + 2] = mag;
        }
    }

    free(copy.data);
}