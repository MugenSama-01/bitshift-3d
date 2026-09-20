#include "utils.h"

void _start(){
    print("Hello World!\n");
    printn(123456);
    print("Fuck me\n");

    __asm__ volatile(
        "syscall"
        :
        :"a" (60),
        "D" (0)
);  
}