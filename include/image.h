#ifndef ARMVISION_IMAGE_H
#define ARMVISION_IMAGE_H

typedef struct {
    unsigned char *data;
    int width;
    int height;
    int row_size;
} Image;

#endif //ARMVISION_IMAGE_H
