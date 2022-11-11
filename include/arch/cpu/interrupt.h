#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <lib/utils.h>
#include <arch/bsp/yellow_led.h>


#define PSR_USR (0x10)
#define PSR_FIQ (0x11)
#define PSR_IRQ (0x12)
#define PSR_SVC (0x13)
#define PSR_ABT (0x17)
#define PSR_UND (0x1b)
#define PSR_SYS (0x1f)

struct dump_regs{
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
    unsigned int sp;
    unsigned int lr;
    unsigned int cpsr;
    unsigned int spsr_irq;
    unsigned int spsr_abt;
    unsigned int spsr_und;
    unsigned int spsr_svc;
    unsigned int sr;
    unsigned int ar;
};

struct mode_regs{
    unsigned int lr;
    unsigned int sp;
    unsigned int spsr;
};

struct interrupt_enables{
    unsigned int en1;
    unsigned int en2;
};

enum EXCEPTION_MODE{
	EX_UND   = 1, // Undefined Instruction
	EX_SVC   = 2, // Supervisor Call
	EX_PFABT = 3, // Prefetch Abort
	EX_DABT  = 4, // Data Abort
	EX_IRQ   = 5  // Interrupt
};


void interrupt_setup();
void reset();
void interrupt(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void reg_dump(enum EXCEPTION_MODE mode, struct dump_regs* regs);
void mode_specific_regs();

#endif