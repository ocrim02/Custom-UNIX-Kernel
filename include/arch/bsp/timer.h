#ifndef _TIMER_H_
#define _TIMER_H_

enum{
    C0 = 0,
    C1 = 1,
    C2 = 2,
    C3 = 3,
};

struct sys_timer_regs{
    unsigned int control;
    unsigned int lower_bits;
    unsigned int higher_bits;
    unsigned int c0;
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
};

void increment_compare(int, int);
void get_pendings_timer();
unsigned int get_timer_value();
void busy_wait(unsigned int);
void loop_wait(unsigned int);
void ack_timer_interrupt(unsigned int);
#endif