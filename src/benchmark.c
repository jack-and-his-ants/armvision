#include "benchmark.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"
#include "filters.h"
#include "filters_asm.h"
#include "filters_neon.h"

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void benchmark_grayscale(Image *img,int iters) {

    double start, end;

    Image copy = *img;
    copy.data = malloc(img->row_size * img->height);
    printf("Grayscale benchmark\n");
    // C VERSION

    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        grayscale(&copy);
    }

    end = get_time();
    float ctime = end - start;
    printf("C grayscale:   %f s\n", ctime);

    // ASM VERSION
    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        grayscale_asm(&copy);
    }

    end = get_time();

    printf("ASM grayscale: %f s, %fx\n", end - start,ctime/(end-start));


    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        grayscale_neon(&copy);
    }

    end = get_time();

    printf("ASM NEON grayscale: %f s, %fx\n", end - start, ctime/(end-start));

    free(copy.data);
}


void benchmark_negative(Image *img, int iters) {

    double start, end;

    Image copy = *img;
    copy.data = malloc(img->row_size * img->height);
    printf("Negative benchmark\n");
    // C VERSION

    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        negative(&copy);
    }

    end = get_time();
    double ctime = end - start;
    printf("C negative:   %f s\n", ctime);

    // ASM VERSION
    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        negative_asm(&copy);
    }

    end = get_time();

    printf("ASM negative: %f s, %fx\n", end - start, ctime/(end-start));

    start = get_time();

    for (int i = 0; i < iters; i++) {
        negative_neon(&copy);
    }

    end = get_time();

    printf("ASM NEON negative: %f s, %fx\n", end - start, ctime/(end-start));

    free(copy.data);
}


void benchmark_monochrome(Image *img,int iters) {

    double start, end;

    Image copy = *img;
    copy.data = malloc(img->row_size * img->height);
    printf("Monochrome benchmark\n");
    // C VERSION

    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();
    char c[] = {'r', 'g', 'b'};
    for (int i = 0; i < iters; i++) {
        monochrome(&copy,c[i%3]);
    }

    end = get_time();

    double ctime = end - start;

    printf("C monochrome:   %f s\n", ctime);

    // ASM VERSION
    memcpy(copy.data, img->data, img->row_size * img->height);

    start = get_time();

    for (int i = 0; i < iters; i++) {
        monochrome_asm(&copy,c[i%3]);
    }

    end = get_time();

    printf("ASM monochrome: %f s, %fx\n", end - start, ctime/(end-start));


    start = get_time();

    for (int i = 0; i < iters; i++) {
        monochrome_neon(&copy,c[i%3]);
    }

    end = get_time();

    printf("ASM neon monochrome: %f s, %fx\n", end - start, ctime/(end-start));

    free(copy.data);
}