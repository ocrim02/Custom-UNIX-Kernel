#ifndef _MAIN_H_
#define _MAIN_H_

//#include <arch/cpu/exception_creator.h>
#include <kernel/syscall.h>
#include <config.h>

void main() __attribute__((weak));
void worker_thread(void*);

#endif