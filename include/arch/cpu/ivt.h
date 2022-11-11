#ifndef _IVT_H_
#define _IVT_H_

#include <arch/cpu/interrupt.h>

void create_prefetch_abort();
void create_supervisor_call();
void create_undefined_instruction();
void create_data_abort();
void set_ivt();

#endif
