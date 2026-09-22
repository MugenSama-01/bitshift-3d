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

struct drm_mode_card_res {
    unsigned long long fb_id_ptr;
    unsigned long long crtc_id_ptr;
    unsigned long long connector_id_ptr;
    unsigned long long encoder_id_ptr;
    unsigned int count_fbs;
    unsigned int count_crtcs;
    unsigned int count_connectors;
    unsigned int count_encoders;
    unsigned int min_width;
    unsigned int max_width;
    unsigned int min_height;
    unsigned int max_height;
};

struct drm_mode_get_connector {
    unsigned long long encoders_ptr;
    unsigned long long modes_ptr;
    unsigned long long props_ptr;
    unsigned long long prop_values_ptr;
    unsigned int count_modes;
    unsigned int count_props;
    unsigned int count_encoders;
    unsigned int encoder_id;
    unsigned int connector_id;
    unsigned int connector_type;
    unsigned int connector_type_id;
    unsigned int connection;
    unsigned int mm_width;
    unsigned int mm_height;
    unsigned int subpixel;
    unsigned int pad;
};

#endif