#include <n7OS/time.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>
#include <unistd.h>
#include <stdio.h>

extern void handler_IT_32();

int timer = 0;

char formated_timer[] = "00:00:00\n";

// Tableau des processus endormis
struct { pid_t pid; int wake_up_time; } sleeping_process[MAX_PROCESS];

void init_timer() {
    init_idt_entry(0x20, (uint32_t)handler_IT_32);
    pic();
    outb(inb(0x21) & 0xfe, 0x21);
    init_sleeping_process();
}

void init_sleeping_process() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        sleeping_process[i].pid = -1;
    }
}

void handler_IT_32_C() {
    outb(0x20, 0x20);
    timer++;
    if (timer % 1000 == 0) {
        update_topbar_uptime();
        update_sleeping_process();
        schedule();
    }
}

void pic() {
    outb(0x34, 0x43);
    outb(0x4A6 & 0xFF, 0x40);
    outb(0x4A6 >> 8, 0x40);
}

//format to hh:mm:ss
char* format_timer() {
    int h = timer / 3600000 % 24;
    int m = (timer / 60000) % 60;
    int s = (timer / 1000) % 60;
    sprintf(formated_timer, "%02d:%02d:%02d\n", h, m, s);
    return formated_timer;
}

void print_timer() {
    printf("uptime : %s", format_timer());
}

void add_sleeping_process(pid_t pid, int seconds) {
    sleeping_process[pid].pid = pid;
    sleeping_process[pid].wake_up_time = timer + seconds * 1000;
}

void update_sleeping_process() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (sleeping_process[i].pid != -1 && sleeping_process[i].wake_up_time <= timer) {
            wake_up(sleeping_process[i].pid);
            sleeping_process[i].pid = -1;
        }
    }
}