#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <lib/utils.h>
#include <config.h>
#include <arch/bsp/yellow_led.h>
#include <arch/bsp/timer.h>
#include <arch/cpu/ivt.h>

void interrupt_setup();
void pendings();
unsigned int get_irq_source();

#endif