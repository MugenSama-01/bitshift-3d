#include "utils.h"

int graphics_win(){
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

int create_buffer(int fd, struct drm_mode_create_dumb *dumb){
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

int create_map(int fd, struct drm_mode_map_dumb *map){
    int result;
    __asm__ volatile(
        "syscall"
        :"=a" (result)
        :"a" (16),
        "D" (fd),
        "S" (0xC01064B3),
        "d" (map)
        :"rcx","r11","memory"
    );
    return (result);
}

unsigned long map_buffer(unsigned long size, int fd, unsigned long long offset){
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

int create_framebuffer(int fd, struct framebuffer *fb){
    int result;

    __asm__ volatile(
        "syscall"
        :"=a" (result)
        :"a" (16),
        "D" (fd),
        "S" (0xC01C64AE),
        "d" (fb)
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

    printn(create_map(fd,&map));
    print("Offset")
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

    int fb_status = create_framebuffer(fd, &fb);
    print("FB Status: ");
    printn(fb_status);
    
    print("Framebuffer ID: ");
    printn(fb.fb_id);

    __asm__ volatile(
        "syscall"
        :
        :"a" (60),
        "D" (0)
);  
}