#ifndef __TIME_H__
#define __TIME_H__

#include <inttypes.h>

void init_timer();
void pic();

void handler_IT_32_C();

char* format_timer();
void print_timer();

#endif