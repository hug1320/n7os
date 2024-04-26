#include <inttypes.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>

extern void handler_IT_32();

int timer = 0;

void init_timer() {
    init_idt_entry(0x20, (uint32_t)handler_IT_32);
    pic();
    outb(inb(0x21)&0xfe, 0x21);
}

void handler_IT_32_C() {
    outb(0x20, 0x20);
    timer++;
}

void pic() {
    outb(0x34, 0x43);
    outb(0x4A6&0xFF, 0x40);
    outb(0x4A6>>8, 0x40);
}

void print_timer() {
    //format hh:mm:ss
    int h = timer / 3600000 % 24;
    int m = (timer / 60000) % 60;
    int s = (timer / 1000) % 60;
    printf("uptime : %02d:%02d:%02d\n", h, m, s);
}