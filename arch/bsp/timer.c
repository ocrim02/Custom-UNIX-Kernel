#include <arch/bsp/timer.h>
#include <lib/utils.h>

#define SYS_TIMER_BASE (0x7E003000 - 0x3F000000)

static volatile
struct sys_timer_regs* const regs = (struct sys_timer_regs *) SYS_TIMER_BASE;

void increment_compare(int interval, int comparator){
    switch(comparator){
    case 0:
        regs->c0 = (regs->lower_bits + (unsigned int) interval) % 0xFFFFFFFF;
        break;
    case 1:
        regs->c1 = (regs->lower_bits + (unsigned int) interval) % 0xFFFFFFFF;
        break;
    case 2:
        regs->c2 = (regs->lower_bits + (unsigned int) interval) % 0xFFFFFFFF;
        break;
    default:
        regs->c3 = (regs->lower_bits + (unsigned int) interval) % 0xFFFFFFFF;
        break;
    }
    
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


