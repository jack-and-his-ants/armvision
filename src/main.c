#include "../include/bmp.h"
#include "../include/filters.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "benchmark.h"


int main(int argc , char ** argv) {
    Image * bmp = load_bmp("../images/snail.bmp");
    benchmark_grayscale(bmp);
    free_bmp(bmp);
    return 0;
}