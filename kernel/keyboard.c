#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#define SCANCODE_TABLE_SIZE 128

extern void handler_IT_33();

char charTable[SCANCODE_TABLE_SIZE] = {
        '\0', '\0', '&', 'e', '"', '\'', '(', '-', 'e', '_', 'c', 'a', ')', '=', '\b', '\t',
        'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', '\0', 'q', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'u', '*', '\0', '\0', 'w', 'x', 'c', 'v',
        'b', 'n', ',', ';', ':', '!', '\0', '\0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

void init_keyboard() {
    init_idt_entry(0x21, (uint32_t)handler_IT_33);
    outb(inb(0x21) & 0xfd, 0x21);
}

void handler_IT_33_C() {
    outb(0x20, 0x20);
    uint8_t scancode = inb(0x60);
    printf("%c", charTable[scancode]);
}