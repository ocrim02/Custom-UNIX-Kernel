#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

#include <stdbool.h>
#include <config.h>

/*
#include <lib/utils.h>
#include <arch/cpu/ivt.h>
#include <arch/cpu/interrupt_handler.h>
*/



#define EN_ARM_TIMER_BASIC (1<<0)
#define EN_SYSTIMER_C1_EN1 (1<<1)
#define EN_SYSTIMER_UART_EN2 (1<<25)

void enable_basic_interrupts_disable_rest(void);
unsigned int interrupt_pos(void);
void pendings(void);

#endif
