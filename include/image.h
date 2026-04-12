#ifndef ARMVISION_IMAGE_H
#define ARMVISION_IMAGE_H

typedef struct {
    int width;
    int height;
    int row_size;
    unsigned char *data;
} Image;

#endif //ARMVISION_IMAGE_H
