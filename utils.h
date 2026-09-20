#ifndef UTILS_H
#define UTILS_H

int len(const char *str);
void print(const char *str);
void printn(int num);

struct drm_mode_create_dumb {
    unsigned int height;
    unsigned int width;
    unsigned int bpp; //bits per pixel
    unsigned int flag;  //0
    //kernal fills them
    unsigned int handle;  //gpus id for our canvas
    unsigned int pitch;  //how many bytes in singel horizontal rows
    unsigned long long size; //total size of canvas in bytes
};

#endif