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

    if (header.type != 0x4D42) {
        printf("Not a BMP file\n");
        fclose(f);
        return NULL;
    }

    if (info.bitsPerPixel != 24) {
        printf("Only 24-bit BMP supported\n");
        fclose(f);
        return NULL;
    }

    if (info.compression != 0) {
        printf("Compressed BMP not supported\n");
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
    fseek(f, header.offset, SEEK_SET);

    fread(img->data, 1, img->row_size * img->height, f);

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