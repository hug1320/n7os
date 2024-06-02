#ifndef __TIME_H__
#define __TIME_H__

#include <inttypes.h>
#include <n7OS/process.h>

void init_timer();
void init_sleeping_process();
void pic();

void handler_IT_32_C();

char* format_timer();
void print_timer();

void add_sleeping_process(pid_t pid, int seconds);
void update_sleeping_process();

#endif