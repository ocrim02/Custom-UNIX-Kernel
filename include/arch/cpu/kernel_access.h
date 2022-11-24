#ifndef _KERNEL_ACCESS_H_
#define _KERNEL_ACCESS_H_

#include <arch/cpu/exception_handler.h>

unsigned int get_processor_mode();
struct mode_regs* get_mode_regs(unsigned int);

#endif