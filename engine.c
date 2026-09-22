#include "utils.h"

int graphics_win(){  //opentind the graphics card file
    const char *path = "/dev/dri/card1";
    int fd;

    __asm__ volatile(
        "syscall"
        :"=a" (fd)
        :"a" (2),
        "D" (path),
        "S" (2),
        "d" (0)
        :"rcx","r11","memory"
    );
    return fd;
}

int create_buffer(int fd, struct drm_mode_create_dumb *dumb){ // creating a buffer memory in the graphics card that does nothing
    int result;

    __asm__ volatile(
        "syscall"
        :"=a" (result)
        :"a" (16),
        "D" (fd),
        "S" (0xC02064B2),
        "d" (dumb)
        :"rcx","r11","memory"
    );
    return result;
}

unsigned long map_buffer(unsigned long size, int fd, unsigned long long offset){  //making display controller recognize it
    unsigned long result;
    register long r10 __asm__("r10") = 1;
    register long r9 __asm__("r9") = offset;
    register long r8 __asm__("r8") = fd;

    __asm__ volatile(
        "syscall"
        :"=a" (result)
        :"a" (9),
        "D" (0),
        "S" (size),
        "d" (3),
        "r" (r10),
        "r" (r9),
        "r" (r8)
        :"rcx","r11","memory"
    );
    return result;
}

int wrapper(int fd, unsigned int hex_code, void *my_struct){
    int result;
    __asm__ volatile(
        "syscall"
        :"=a" (result)
        :"a" (16),
        "D" (fd),
        "S" (hex_code),
        "d" (my_struct)
        :"rcx","r11","memory"
    );
    return result;
}

void _start(){
    int fd=graphics_win();
    printn(fd);

    struct drm_mode_create_dumb dumb;

    dumb.width = 800;
    dumb.height=600;
    dumb.bpp=32;
    dumb.flag = 0;
    dumb.handle = 0;
    dumb.pitch = 0;
    dumb.size = 0;

    int out= create_buffer(fd,&dumb);

    print("ioctl status: ");
    printn(out);

    print("Canvas GPU Handle: ");
    printn(dumb.handle);

    print("Canvas Size (Bytes): ");
    printn(dumb.size);

    struct drm_mode_map_dumb map;

    map.handle=dumb.handle;
    map.pad = 0;
    map.offset=0;

    printn(wrapper(fd,0xC01064B3,&map)); // saying cpu about any memory in this block send it to the gpus vram not ram
    print("Offset");
    printn(map.offset);

    unsigned long map_ptr=map_buffer(dumb.size , fd , map.offset);
    print("canvus Memory Address:");
    printn(map_ptr);

    struct framebuffer fb;
    fb.fb_id=0;
    fb.width=800;
    fb.height=600;
    fb.pitch=dumb.pitch;
    fb.bpp=32;
    fb.depth=24;
    fb.handle=dumb.handle;

    int fb_status = wrapper(fd,0xC01C64AE, &fb); // create frame buffer
    print("FB Status: ");
    printn(fb_status);
    
    print("Framebuffer ID: ");
    printn(fb.fb_id);

    struct drm_mode_card_res res;
    res.fb_id_ptr = 0;
    res.crtc_id_ptr = 0;
    res.connector_id_ptr = 0;
    res.encoder_id_ptr = 0;
    res.count_fbs = 0;
    res.count_crtcs = 0;
    res.count_connectors = 0;
    res.count_encoders = 0;
    res.min_width = 0;
    res.max_width = 0;
    res.min_height = 0;
    res.max_height = 0;

    printn(wrapper(fd ,0xC04064A0, &res)); //the the info about the resources
    printn(res.count_crtcs);
    printn(res.count_connectors);

    unsigned int crtc_ids[res.count_crtcs];
    unsigned int conn_ids[res.count_connectors];

    res.crtc_id_ptr = (unsigned long long)crtc_ids;
    res.connector_id_ptr = (unsigned long long)conn_ids;
    res.count_fbs=0;
    res.count_encoders=0;

    printn(wrapper(fd ,0xC04064A0, &res)); //duel call to get the connector IDs now

    struct drm_mode_get_connector connector;
    connector.encoders_ptr = 0;
    connector.modes_ptr = 0;
    connector.props_ptr = 0;
    connector.prop_values_ptr = 0;
    connector.count_modes = 0;
    connector.count_props = 0;
    connector.count_encoders = 0;
    connector.encoder_id = 0;
    connector.connector_id = conn_ids[0]; 
    connector.connector_type = 0;
    connector.connector_type_id = 0;
    connector.connection = 0;
    connector.mm_width = 0;
    connector.mm_height = 0;
    connector.subpixel = 0;
    connector.pad = 0;

    printn(wrapper(fd ,0xC05064A7, &connector)); 
    printn(connector.connection);
    print("Width (mm): ");
    printn(connector.mm_width);
    print("Height (mm): ");
    printn(connector.mm_height);
    

    __asm__ volatile(
        "syscall"
        :
        :"a" (60),
        "D" (0)
);  
}