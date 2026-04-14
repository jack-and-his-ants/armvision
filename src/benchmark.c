#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"
#include "filters.h"
#include "filters_asm.h"

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void benchmark_grayscale(Image *img) {
    const int ITER = 1000;

    double start, end;

    Image copy = *img;
    copy.data = malloc(img->row_size * img->height);

    // C VERSION

    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < ITER; i++) {
        grayscale(&copy);
    }

    end = get_time();

    printf("C grayscale:   %f s\n", end - start);

    // ASM VERSION
    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < ITER; i++) {
        grayscale_asm(&copy);
    }

    end = get_time();

    printf("ASM grayscale: %f s\n", end - start);

    free(copy.data);
}