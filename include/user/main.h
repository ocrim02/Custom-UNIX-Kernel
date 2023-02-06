#ifndef _MAIN_H_
#define _MAIN_H_

#include <user/syscall.h>
#include <config.h>
#include <arch/cpu/mmu.h>

void main() __attribute__((weak));
void worker_thread(void*);
void worker_process(void* arg);
int uint_to_dec_str (char* string, unsigned int value);

#endif