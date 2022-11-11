#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <lib/utils.h>
#include <arch/bsp/yellow_led.h>

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

#endif