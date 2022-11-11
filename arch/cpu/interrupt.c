#include <arch/cpu/interrupt.h>

#define INTERRUPT_BASE (0x7E00B000 - 0x3F000000)
#define ENABLE_IRQ_1 0x210
#define ENABLE_IRQ_2 0x214
#define ENABLE_IRQ_BASIC 0x218

void interrupt_setup(){
    //struct uart_regs* const regs = (struct uart_regs *) UART_BASE;
    //unsigned int* enable_1 = (unsigned int*) INTERRUPT_BASE + ENABLE_IRQ_1;
    //enable_1 = (unsigned int) 0x2000000F;
    //unsigned int* enable_2 = (unsigned int*) INTERRUPT_BASE + ENABLE_IRQ_2;
    //enable_2 = (unsigned int) 0x42FF6800;

    struct interrupt_enables* const enable = (struct interrupt_enables*) INTERRUPT_BASE + ENABLE_IRQ_1;
    enable->en1 = (unsigned int) 0x2000000F;
    enable->en2 = (unsigned int) 0x42FF6800;
}

void reset(){
    kprintf("reset\n");
    while(1);
}

void interrupt(enum EXCEPTION_MODE mode, struct dump_regs * regs){
    switch(mode){
        case EX_UND:
            kprintf("Undefined Instruction\n");
            reg_dump(mode, regs);
            reset();
            break;
        case EX_SVC:
            kprintf("Supervisor Call\n");
            reg_dump(mode, regs);
            reset();
            break;
        case EX_PFABT:
            kprintf("Prefetch Abort\n");
            reg_dump(mode, regs);
            reset();
            break;
        case EX_DABT:
            kprintf("Data Abort\n");
            reg_dump(mode, regs);
            reset();
            break;
        case EX_IRQ:
            kprintf("Interrupt\n");
            reg_dump(mode, regs);
            break;
        default:
            kprintf("Undefined Exception\n");
            break;
    }
}

void mode_specific_regs(){
    kprintf(">> Modusspezifische Register <<\n");
    //kprintf("User/System | LR: 0x%08x | SP: 0x%08x\n", regs->usr_lr, regs->usr_sp);
    struct mode_regs* regs = (struct mode_regs*) get_mode_regs(PSR_IRQ);
    kprintf("IRQ         | LR: 0x%08x | SP: 0x%08x | SPSR: 0x%08x\n", regs->lr, regs->sp, regs->spsr);
    regs = (struct mode_regs*) get_mode_regs(PSR_ABT);
    kprintf("Abort       | LR: 0x%08x | SP: 0x%08x | SPSR: 0x%08x\n", regs->lr, regs->sp, regs->spsr);
    regs = (struct mode_regs*) get_mode_regs(PSR_UND);
    kprintf("Undefined   | LR: 0x%08x | SP: 0x%08x | SPSR: 0x%08x\n", regs->lr, regs->sp, regs->spsr);
    regs = (struct mode_regs*) get_mode_regs(PSR_SVC);
    kprintf("Supervisor  | LR: 0x%08x | SP: 0x%08x | SPSR: 0x%08x\n", regs->lr, regs->sp, regs->spsr);
}

void reg_dump(enum EXCEPTION_MODE mode, struct dump_regs * regs){
    kprintf("############## EXCEPTION ##############\n");
    switch (mode){
    case EX_DABT:
        kprintf("Data Abort an Adresse: 0x%08x\n", regs->ar);
        kprintf("Data Fault Status Register: 0x%08x\n", regs->sr);
        kprintf("Data Fault Adress Register: 0x%08x\n", regs->ar);
        break;
    case EX_UND:
        //kprintf("Undefined Instruction an Adresse: 0x%08x\n", regs->pc);
        break;
    case EX_PFABT:
        //kprintf("Prefetch Abort an Adresse: 0x%08x\n", regs->pc);
        break;
    case EX_IRQ:
        //kprintf("Interrupt an Adresse: 0x%08x\n", regs->pc);
        break;
    case EX_SVC:
        //kprintf("Supervisor Call an Adresse: 0x%08x\n", regs->pc);
        break;
    
    default:
        break;
    }

    kprintf("\n");
    kprintf(">> Registerschnappschuss <<\n");
    kprintf("R0: 0x%08x  R5: 0x%08x  R10: 0x%08x\n", regs->r0, regs->r5, regs->r10);
    kprintf("R1: 0x%08x  R6: 0x%08x  R11: 0x%08x\n", regs->r1, regs->r6, regs->r11);
    kprintf("R2: 0x%08x  R7: 0x%08x  R12: 0x%08x\n", regs->r2, regs->r7, regs->r12);
    kprintf("R3: 0x%08x  R8: 0x%08x\n", regs->r3, regs->r8);
    kprintf("R4: 0x%08x  R9: 0x%08x\n", regs->r4, regs->r9);
    kprintf("\n");

    mode_specific_regs();
}