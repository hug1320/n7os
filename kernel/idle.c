#include <stdio.h>
#include <n7OS/process.h>
#include <n7OS/cpu.h>

void idle() {
    printf("Idle\n");
    schedule();
    while (1) {
        hlt();
    }
}