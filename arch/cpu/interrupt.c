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

static volatile
struct interrupt_enables* const enable = (struct interrupt_enables*) (INTERRUPT_BASE + BASIC_PENDING_OFFSET);



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



