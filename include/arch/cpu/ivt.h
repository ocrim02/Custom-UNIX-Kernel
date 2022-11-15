#ifndef _IVT_H_
#define _IVT_H_

#include <arch/cpu/interrupt.h>
#include <lib/utils.h>

void create_prefetch_abort();
void create_supervisor_call();
void create_undefined_instruction();
void create_data_abort();
struct mode_regs* get_mode_regs(unsigned int);
void set_ivt();
unsigned int get_processor_mode();
void enable_interrupts();
unsigned int mem_test(unsigned int, unsigned int, unsigned int, unsigned int);

#endif
