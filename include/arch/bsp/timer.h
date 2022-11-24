#ifndef _TIMER_H_
#define _TIMER_H_

void increment_compare(int);
void get_pendings_timer();
unsigned int get_timer_value();
void busy_wait(unsigned int);
void ack_timer_interrupt(unsigned int);
#endif