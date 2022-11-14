#ifndef TIMER_H
#define TIMER_H

void init_timer(int);
unsigned int get_timer_value();
void wait(unsigned int);
void reset_compare(unsigned int);
#endif