#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <lib/utils.h>
#include <config.h>
#include <arch/bsp/yellow_led.h>
#include <arch/bsp/timer.h>
#include <arch/cpu/ivt.h>



enum INTERRUPT_SOURCE{
    SYS_TIMER_1 = 0,
    SYS_TIMER_2 = 1,
    SYS_TIMER_3 = 2,
    SYS_TIMER_4 = 3,
    UART = 57
};



unsigned int get_irq_regdump();
void switch_irq_regdump();
void switch_loop_mode();
void interrupt_setup();
void pendings();
unsigned int get_irq_source();

#endif