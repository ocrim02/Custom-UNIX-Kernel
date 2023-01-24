#ifndef _EXCEPTION_HANDLER_H_
#define _EXCEPTION_HANDLER_H_

#include <arch/cpu/cpu.h>
#include <arch/cpu/interrupt.h>
#include <arch/cpu/kernel_access.h>
#include <kernel/thread.h>
#include <user/main.h>
#include <arch/cpu/syscall_routines.h>
#include <arch/cpu/mmu.h>

enum EXCEPTION_MODE{
	EX_UND   = 1, // Undefined Instruction
	EX_SVC   = 2, // Supervisor Call
	EX_PFABT = 3, // Prefetch Abort
	EX_DABT  = 4, // Data Abort
	EX_IRQ   = 5, // Interrupt
    EX_FIQ   = 6, // Fast INterrupt
    EX_UNU   = 7  // Unused
};

enum INTERRUPT_SOURCE{
    SYS_TIMER_1 = 0,
    SYS_TIMER_2 = 1,
    SYS_TIMER_3 = 2,
    SYS_TIMER_4 = 3,
    UART = 57
};

struct dump_regs{
    unsigned int sp;
    unsigned int lr;
    unsigned int spsr;
    unsigned int sr;
    unsigned int ar;
    unsigned int r[13];
    unsigned int pc;
    
};

struct mode_regs{
    unsigned int lr;
    unsigned int sp;
    unsigned int spsr;
};

void switch_irq_regdump();
void switch_loop_mode();
void exception(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void reg_dump(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void spsr_info(unsigned int);
void data_fault_source(unsigned int);
void instruction_fault_source(unsigned int);
void stop();

#endif