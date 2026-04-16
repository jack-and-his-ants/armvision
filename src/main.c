#include "../include/bmp.h"
#include "../include/filters.h"
#include "benchmark.h"
#include "filters_asm.h"
#include "filters_neon.h"


int main(int argc , char ** argv) {
    Image * bmp = load_bmp("../images/greenland_grid_velo.bmp");
    negative_neon(bmp);
    save_bmp(bmp,"test2.bmp");
    benchmark_negative(bmp);
    // benchmark_monochrome(bmp);
    // benchmark_grayscale(bmp);
    free_bmp(bmp);
    return 0;
}
