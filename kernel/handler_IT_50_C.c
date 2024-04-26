#include <inttypes.h>
#include <n7OS/irq.h>

extern void handler_IT_50();

void init_IT_50() {
    init_idt_entry(50, (uint32_t)handler_IT_50);
}

void handler_IT_50_C() {
    printf("interruption :\t IT_50 catched ! \n");
}