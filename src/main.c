#include "../include/bmp.h"
#include "../include/filters.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "benchmark.h"
#include "filters_asm.h"


int main(int argc , char ** argv) {
    Image * bmp = load_bmp("../images/snail.bmp");
    save_bmp(bmp,"test2.bmp");
    benchmark_monochrome(bmp);
    benchmark_grayscale(bmp);
    free_bmp(bmp);
    return 0;
}
