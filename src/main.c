#include "../include/bmp.h"
#include "../include/filters.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc , char ** argv) {
    Image * bmp = load_bmp("snail.bmp");
    save_bmp(bmp, "test.bmp");
    free_bmp(bmp);
    return 0;
}