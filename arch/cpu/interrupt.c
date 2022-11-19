#include <arch/cpu/interrupt.h>

#define INTERRUPT_BASE (0x7E00B000 - 0x3F000000)
#define BASIC_PENDING_OFFSET 0x200
#define ENABLE_IRQ_1 0x210
#define ENABLE_IRQ_2 0x214
#define ENABLE_IRQ_BASIC 0x218

struct interrupt_enables{
    unsigned int basic_pending;
    unsigned int irq_pending_1;
    unsigned int irq_pending_2;
    unsigned int unused;
    unsigned int en1;
    unsigned int en2;
    unsigned int base_enable;
};

volatile unsigned int irq_regdump = 0;
volatile unsigned int character_loop_mode = 0;

static volatile
struct interrupt_enables* const enable = (struct interrupt_enables*) (INTERRUPT_BASE + BASIC_PENDING_OFFSET);
/*
unsigned int get_irq_regdump(){
    return irq_regdump;
}
*/
/*
void switch_irq_regdump(){
    if(irq_regdump){
		irq_regdump = 0;
	}
	else{
		irq_regdump = 1;
	}
}
*/
/*
void switch_loop_mode(){
    if(character_loop_mode){
		character_loop_mode = 0;
	}
	else{
		character_loop_mode = 1;
	}
}
*/
void interrupt_setup(){
    enable->en1 = 1 << 1;
    enable->en2 = 1 << 25;
    enable->base_enable = 1<<0;
}

void pendings(){
    kprintf("Basic: %x\n", enable->basic_pending);
    kprintf("IRQ_1 %x\n", enable->irq_pending_1);
    kprintf("IRQ_2 %x\n", enable->irq_pending_2);
}

unsigned int get_irq_source(){
    unsigned int position = __builtin_ctz(enable->irq_pending_1);
    if(position == 32){
        return position + __builtin_ctz(enable->irq_pending_2);
    }
    else{
        return position;
    }
    
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
            reg_dump(mode, regs);
            reset();
            break;
        case EX_IRQ:
            if(irq_regdump){
                reg_dump(mode, regs);
            }
            /*
            switch(get_irq_source()){
                case SYS_TIMER_2:
                    if(character_loop_mode == 1){
                        kprintf("!\n");

                    }
                    increment_compare(TIMER_INTERVAL);
                    ack_timer_interrupt(1);
                    break;
                case UART:
                    put_ring_buffer(read_uart());
                    break;

            }
            */
            break;
            

        default:
            kprintf("Undefined Exception\n");
            break;
    }
}

void spsr_info(unsigned int spsr){
    if(spsr == 0){
        kprintf(" | SPSR: ____ _ ___     Invalid 0x00000000\n");
    }
    else{
        kprintf(" | SPSR: ");
        if(read_masked(spsr, 31, 31) == 1){
            kprintf("N");
        }else{
            kprintf("_");
        }
        if(read_masked(spsr, 30, 30) == 1){
            kprintf("Z");
        }else{
            kprintf("_");
        }
        if(read_masked(spsr, 29, 29) == 1){
            kprintf("C");
        }else{
            kprintf("_");
        }
        if(read_masked(spsr, 28, 28) == 1){
            kprintf("V ");
        }else{
            kprintf("_ ");
        }
        if(read_masked(spsr, 9, 9) == 1){
            kprintf("E ");
        }else{
            kprintf("_ ");
        }
        if(read_masked(spsr, 7, 7) == 1){
            kprintf("I");
        }else{
            kprintf("_");
        }
        if(read_masked(spsr, 6, 6) == 1){
            kprintf("F");
        }else{
            kprintf("_");
        }
        if(read_masked(spsr, 5, 5) == 1){
            kprintf("T");
        }else{
            kprintf("_");
        }
        kprintf("  Supervisor 0x%08x\n", spsr);
    }
}


void reg_dump(enum EXCEPTION_MODE mode, struct dump_regs * regs){
    kprintf("############## EXCEPTION ##############\n");
    switch (mode){
        case EX_DABT:
            kprintf("Data Abort an Adresse: 0x%08x\n", regs->pc);
            kprintf("Data Fault Status Register: 0x%08x", regs->sr);
            data_fault_source(regs->sr);
            kprintf("Data Fault Adress Register: 0x%08x\n", regs->ar);
            break;
        case EX_UND:
            kprintf("Undefined Instruction an Adresse: 0x%08x\n", regs->pc);
            break;
        case EX_PFABT:
            kprintf("Prefetch Abort an Adresse: 0x%08x\n", regs->pc);
            kprintf("Prefetch Fault Status Register: 0x%08x", regs->sr);
            instruction_fault_source(regs->sr);
            kprintf("Prefetch Fault Adress Register: 0x%08x\n", regs->ar);
            break;
        case EX_IRQ:
            kprintf("Interrupt an Adresse: 0x%08x\n", regs->pc);
            break;
        case EX_SVC:
            kprintf("Supervisor Call an Adresse: 0x%08x\n", regs->pc);
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

    kprintf(">> Modusspezifische Register <<\n");
    kprintf("User/System | LR: 0x%08x | SP: 0x%08x\n", regs->lr, regs->sp);
    struct mode_regs* mod_regs = (struct mode_regs*) get_mode_regs(PSR_IRQ);
    kprintf("IRQ         | LR: 0x%08x | SP: 0x%08x", mod_regs->lr, mod_regs->sp);
    spsr_info(mod_regs->spsr);
    mod_regs = (struct mode_regs*) get_mode_regs(PSR_ABT);
    kprintf("Abort       | LR: 0x%08x | SP: 0x%08x", mod_regs->lr, mod_regs->sp);
    spsr_info(mod_regs->spsr);
    mod_regs = (struct mode_regs*) get_mode_regs(PSR_UND);
    kprintf("Undefined   | LR: 0x%08x | SP: 0x%08x", mod_regs->lr, mod_regs->sp);
    spsr_info(mod_regs->spsr);
    mod_regs = (struct mode_regs*) get_mode_regs(PSR_SVC);
    kprintf("Supervisor  | LR: 0x%08x | SP: 0x%08x", mod_regs->lr, mod_regs->sp);
    spsr_info(mod_regs->spsr);
}


void data_fault_source(unsigned int status){
    switch(read_masked(status, 4, 0)){
        case 0x0:
            kprintf(" -> No function, reset value\n");
            break;
        case 0x1:
            kprintf(" -> Alignment fault\n");
            break;
        case 0x2:
            kprintf(" -> Debug event fault\n");
            break;
        case 0x3:
            kprintf(" -> Access Flag fault on Section\n");
            break;
        case 0x4:
            kprintf(" -> Cache maintenance operation fault\n");
            break;
        case 0x5:
            kprintf(" -> Translation fault on section\n");
            break;
        case 0x6:
            kprintf(" -> Access Flag fault on Page\n");
            break;
        case 0x7:
            kprintf(" -> Translation fault on Page\n");
            break;
        case 0x8:
            kprintf(" -> Precise External Abort\n");
            break;
        case 0x9:
            kprintf(" -> Domain fault on section\n");
            break;
        case 0xb:
            kprintf(" -> Domain fault on Page\n");
            break;
        case 0xc:
            kprintf(" -> External abort on Section\n");
            break;
        case 0xd:
            kprintf(" -> Permission fault on Page\n");
            break;
        case 0xe:
            kprintf(" -> External abort on Page\n");
            break;
        case 0xf:
            kprintf(" -> Permission fault on Page\n");
            break;
        case 0x16:
            kprintf(" -> Imprecise External Abort\n");
            break;
        default: 
            kprintf(" -> No function\n");
            break;
    }
}

void instruction_fault_source(unsigned int status){
    switch(read_masked(status, 4, 0)){
        case 0x0:
            kprintf(" -> No function, reset value\n");
            break;
        case 0x2:
            kprintf(" -> Debug event fault\n");
            break;
        case 0x3:
            kprintf(" -> Access Flag fault on Section\n");
            break;
        case 0x5:
            kprintf(" -> Translation fault on section\n");
            break;
        case 0x6:
            kprintf(" -> Access Flag fault on Page\n");
            break;
        case 0x7:
            kprintf(" -> Translation fault on Page\n");
            break;
        case 0x8:
            kprintf(" -> Precise External Abort\n");
            break;
        case 0x9:
            kprintf(" -> Domain fault on section\n");
            break;
        case 0xb:
            kprintf(" -> Domain fault on Page\n");
            break;
        case 0xc:
            kprintf(" -> External abort on Section\n");
            break;
        case 0xd:
            kprintf(" -> Permission fault on Section\n");
            break;
        case 0xe:
            kprintf(" -> External abort on Page\n");
            break;
        case 0xf:
            kprintf(" -> Permission fault on Page\n");
            break;
        default: 
            kprintf(" -> No function\n");
            break;
    }
}
