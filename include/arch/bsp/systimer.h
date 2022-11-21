#ifndef SYSTIMER_H
#define SYSTIMER_H

#include <stdbool.h>
#include <config.h>
//#include <lib/utils.h>

#define ONES_32BIT 0xFFFFFFFF

//setzt den timer c1
void init_systimer(int timer, unsigned int time);

//init c 1 2 3 machen wenn 0 geht 

//findet heraus welcher timer den interrupt erzeugt hat
void timer_irq_solver(int timer, int time);

#endif


