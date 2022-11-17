#ifndef SYSTIMER_H
#define SYSTIMER_H

#include <stdbool.h>
#include <config.h>
#include <lib/utils.h>

//setzt den timer c0
void init_systimer_c0(unsigned int timer);

//init c 1 2 3 machen wenn 0 geht 

//findet heraus welcher timer den interrupt erzeugt hat
void timer_irq_solver(void);

#endif


