#include <arch/bsp/timer.h>
#include <lib/utils.h>

#define SYS_TIMER_BASE (0x7E003000 - 0x3F000000)

struct sys_timer_regs{
    unsigned int control;
    unsigned int lower_bits;
    unsigned int higher_bits;
    unsigned int c0;
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
};

static volatile
struct sys_timer_regs* const regs = (struct sys_timer_regs *) SYS_TIMER_BASE;

void increment_compare(int interval){
    regs->c1 = (regs->lower_bits + (unsigned int) interval) % 0xFFFFFFFF;
}

void get_pendings_timer(){
    kprintf("pending timer: %x\n", regs->control);
}

unsigned int get_timer_value(){
    return regs->lower_bits;
}

void ack_timer_interrupt(unsigned int cmp){
    regs->control |= 1 << cmp;
}

// do not use for precise timings
void busy_wait(unsigned int duration){
    unsigned int start_time = regs->lower_bits;
    unsigned int stop_time = (start_time + duration) % 0xFFFFFFFF;
    
    if(start_time < stop_time){
        while(regs->lower_bits < stop_time){}
    }
    else{
        unsigned int start_upper = regs->higher_bits;
        while(start_upper == regs->higher_bits || regs->lower_bits < stop_time){}
    }
}


