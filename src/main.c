#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "bmp.h"
#include "filters.h"
#include "filters_asm.h"
#include "filters_neon.h"
#include "benchmark.h"

void print_usage(char *prog_name) {
    printf("Usage: %s -i <in.bmp> -o <out.bmp> -f <filter> [options]\n", prog_name);
    printf("Filters: negative, grayscale, monochrome, sobel, blur, gaussian_blur\n");
    printf("Modes (-m): c (default), asm, neon\n");
    printf("Options:\n");
    printf("  -c <r/g/b>  Channel for monochrome filter\n");
}

int main(int argc, char **argv) {
        char *input_path = NULL, *output_path = NULL, *filter = NULL;
        char *mode = "c", *channel_arg = "g";
        int kernel_size = 3;
        int iterations = 100;
        int opt;

        while ((opt = getopt(argc, argv, "i:o:f:m:c:s:n:h")) != -1) {
            switch (opt) {
                case 'i': input_path = optarg; break;
                case 'o': output_path = optarg; break;
                case 'f': filter = optarg; break;
                case 'm': mode = optarg; break;
                case 'c': channel_arg = optarg; break;
                case 's': kernel_size = atoi(optarg); break;
                case 'n': iterations = atoi(optarg); break;
                case 'h': print_usage(argv[0]); return 0;
                default: return 1;
            }
        }

        if (!input_path || !filter) {
            fprintf(stderr, "Error: Input and filter are required.\n");
            return 1;
        }

        Image *bmp = load_bmp(input_path);
        if (!bmp) return 1;

        if (strcmp(mode, "bench") == 0) {
            printf("[!] Starting benchmark: %s (%d iterations)\n", filter, iterations);
            if (strcmp(filter, "negative") == 0) benchmark_negative(bmp, iterations);
            else if (strcmp(filter, "grayscale") == 0) benchmark_grayscale(bmp, iterations);
            else if (strcmp(filter, "monochrome") == 0) benchmark_monochrome(bmp, iterations);
            free_bmp(bmp);
            return 0;
        }

        printf("[*] Executing %s in %s mode...\n", filter, mode);

        // FILTERS
        if (strcmp(filter, "negative") == 0) {
            if (strcmp(mode, "neon") == 0) negative_neon(bmp);
            else if (strcmp(mode, "asm") == 0) negative_asm(bmp);
            else negative(bmp);
        }
        else if (strcmp(filter, "grayscale") == 0) {
            if (strcmp(mode, "neon") == 0) grayscale_neon(bmp);
            else if (strcmp(mode, "asm") == 0) grayscale_asm(bmp);
            else grayscale(bmp);
        }
        else if (strcmp(filter, "monochrome") == 0) {
            char c = channel_arg[0];
            if (strcmp(mode, "neon") == 0) monochrome_neon(bmp, c);
            else if (strcmp(mode, "asm") == 0) monochrome_asm(bmp, c);
            else monochrome(bmp, c);
        }
        else if (strcmp(filter, "sobel") == 0) {
            sobel(bmp);
        }
        else if (strcmp(filter, "blur") == 0) {
            blur(bmp,kernel_size);
        }
        else if (strcmp(filter, "gaussian_blur") == 0) {
            gaussian_blur(bmp, kernel_size);
        }
        else {
            fprintf(stderr, "Unknown filter: %s\n", filter);
        }

        if (output_path) {
            if (save_bmp(bmp, output_path) == 0) printf("[+] Saved: %s\n", output_path);
        }

        free_bmp(bmp);
        return 0;
}