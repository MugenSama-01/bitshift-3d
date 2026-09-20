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

    __asm__ volatile(
        "syscall"
        :
        :"a" (60),
        "D" (0)
);  
}