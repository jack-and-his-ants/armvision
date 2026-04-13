#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp.h"

#pragma pack(push, 1)

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColors;
} BMPInfoHeader;

#pragma pack(pop)

// LOAD

Image* load_bmp(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    BMPHeader header;
    BMPInfoHeader info;

    fread(&header, sizeof(header), 1, f);
    fread(&info, sizeof(info), 1, f);

    // is BMP
    if (header.type != 0x4D42) {
        printf("Not a BMP file\n");
        fclose(f);
        return NULL;
    }

    if (info.compression != 0) {
        printf("Compressed BMP not supported\n");
        fclose(f);
        return NULL;
    }

    int is_8bit = (info.bitsPerPixel == 8);
    int is_24bit = (info.bitsPerPixel == 24);

    if (!is_8bit && !is_24bit) {
        printf("Only 8-bit and 24-bit BMP supported\n");
        fclose(f);
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    if (!img) {
        fclose(f);
        return NULL;
    }

    img->width = info.width;
    img->height = info.height;

    img->row_size = (img->width * 3 + 3) & (~3);

    img->data = malloc(img->row_size * img->height);
    if (!img->data) {
        free(img);
        fclose(f);
        return NULL;
    }

    // reading palette of colors
    uint8_t palette[256][4];

    if (is_8bit) {
        int palette_size = info.colorsUsed ? info.colorsUsed : 256;
        fread(palette, 4, palette_size, f);
    }

    fseek(f, header.offset, SEEK_SET);

    // 24-bit handling
    if (is_24bit) {
        fread(img->data, 1, img->row_size * img->height, f);
    }
    // 8 - bit handling
    if (is_8bit) {
        int row_size_8 = (img->width + 3) & (~3);
        uint8_t *row = malloc(row_size_8);

        for (int y = 0; y < img->height; y++) {
            fread(row, 1, row_size_8, f);

            for (int x = 0; x < img->width; x++) {
                uint8_t idx = row[x];

                int z = y * img->row_size + x * 3;

                img->data[z]     = palette[idx][0]; // B
                img->data[z + 1] = palette[idx][1]; // G
                img->data[z + 2] = palette[idx][2]; // R
            }
        }

        free(row);
    }

    fclose(f);
    return img;
}

// SAVE

int save_bmp(Image *img, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return 0;
    }

    BMPHeader header;
    BMPInfoHeader info;

    int row_size = (img->width * 3 + 3) & (~3);
    int data_size = row_size * img->height;

    header.type = 0x4D42; // "BM"
    header.size = 54 + data_size;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = 54;

    info.size = 40;
    info.width = img->width;
    info.height = img->height;
    info.planes = 1;
    info.bitsPerPixel = 24;
    info.compression = 0;
    info.imageSize = data_size;
    info.xPixelsPerMeter = 0;
    info.yPixelsPerMeter = 0;
    info.colorsUsed = 0;
    info.importantColors = 0;

    fwrite(&header, sizeof(header), 1, f);
    fwrite(&info, sizeof(info), 1, f);
    fwrite(img->data, 1, data_size, f);

    fclose(f);
    return 1;
}

// FREE

void free_bmp(Image *img) {
    if (!img) return;
    free(img->data);
    free(img);
}