#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <lib/utils.h>
#include <config.h>
#include <arch/bsp/yellow_led.h>
#include <arch/cpu/timer.h>
#include <arch/cpu/ivt.h>


#define PSR_USR (0x10)
#define PSR_FIQ (0x11)
#define PSR_IRQ (0x12)
#define PSR_SVC (0x13)
#define PSR_ABT (0x17)
#define PSR_UND (0x1b)
#define PSR_SYS (0x1f)

struct dump_regs{
    unsigned int sp;
    unsigned int lr;
    unsigned int spsr;
    unsigned int sr;
    unsigned int ar;
    unsigned int r0;
    unsigned int r1;
    unsigned int r2;
    unsigned int r3;
    unsigned int r4;
    unsigned int r5;
    unsigned int r6;
    unsigned int r7;
    unsigned int r8;
    unsigned int r9;
    unsigned int r10;
    unsigned int r11;
    unsigned int r12; 
    unsigned int pc;
    
};

struct mode_regs{
    unsigned int lr;
    unsigned int sp;
    unsigned int spsr;
};

enum INTERRUPT_SOURCE{
    SYS_TIMER_1 = 0,
    SYS_TIMER_2 = 1,
    SYS_TIMER_3 = 2,
    SYS_TIMER_4 = 3,
    UART = 57
};

enum EXCEPTION_MODE{
	EX_UND   = 1, // Undefined Instruction
	EX_SVC   = 2, // Supervisor Call
	EX_PFABT = 3, // Prefetch Abort
	EX_DABT  = 4, // Data Abort
	EX_IRQ   = 5, // Interrupt
    EX_FIQ   = 6, // Fast INterrupt
    EX_UNU   = 7  // Unused
};

unsigned int get_irq_regdump();
void switch_irq_regdump();
void switch_loop_mode();
void data_fault_source(unsigned int);
void instruction_fault_source(unsigned int);
void interrupt_setup();
void reset();
void interrupt(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void reg_dump(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void spsr_info(unsigned int);
void pendings();
unsigned int get_irq_source();

#endif