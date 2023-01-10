#ifndef _SYSCALL_ROUTINES_H_
#define _SYSCALL_ROUTINES_H_

#include <kernel/thread.h>

typedef struct dump_regs dump_regs;

void getc_routine(struct dump_regs * regs);
void putc_routine(struct dump_regs * regs);
void exit_routine(struct dump_regs * regs);
void sleep_routine(struct dump_regs * regs);
void thread_create_routine(struct dump_regs * regs);

#endif