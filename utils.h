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

struct drm_mode_map_dumb {
    unsigned int handle;
    unsigned int pad;
    unsigned long long offset;
};

struct framebuffer {
    unsigned int fb_id;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int bpp;
    unsigned int depth;
    unsigned int handle;
};

#endif