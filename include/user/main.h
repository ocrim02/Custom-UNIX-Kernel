#ifndef _MAIN_H_
#define _MAIN_H_

//#include <arch/cpu/exception_creator.h>
#include <user/syscall.h>
#include <config.h>
#include <arch/cpu/mmu.h>

void main() __attribute__((weak));
void worker_thread(void*);

#endif