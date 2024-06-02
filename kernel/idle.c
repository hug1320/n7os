#include <stdio.h>
#include <n7OS/process.h>
#include <n7OS/cpu.h>

void idle() {
    printf("You can exit kernel with ctrl + c or ctrl + d\n");
    printf("Idle\n");
    schedule();
    while (1) {
        hlt();
    }
}