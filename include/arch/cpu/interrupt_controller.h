#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

#include <stdbool.h>
#include <config.h>

/*
#include <lib/utils.h>
#include <arch/cpu/ivt.h>
#include <arch/cpu/interrupt_handler.h>
*/

#define ONES_32BIT 0xFFFFFFFF
#define ZEROS_32BIT 0x00000000

#define EN_ARM_TIMER_BASIC (1<<0)
#define EN_SYSTIMER_C1_EN1 (1<<1)
#define EN_SYSTIMER_UART_EN2 (1<<25)
#define BIT_IN_PEND1 (1<<8)
#define BIT_IN_PEND2 (1<<9)

void enable_irq(unsigned int basic_irqs, unsigned int irqs_1, unsigned int irqs_2);
void disable_irq(unsigned int basic_irqs, unsigned int irqs_1, unsigned int irqs_2);
unsigned int irq_basis_pos(void);
unsigned int irq_pend1_pos(void);
unsigned int irq_pend2_pos(void);

#endif
