#ifndef PL011_UART_H
#define PL011_UART_H

#include <stdbool.h>
#include <config.h>
#include <lib/utils.h>

struct uart_regs{
    unsigned int data;
    unsigned int unused0[5];
    unsigned int fr;
    unsigned int unused1[7];
    unsigned int imsc;
    unsigned int ris;
    unsigned int mis;
    unsigned int icr;
};


void setup_int_uart();
void get_pendings_uart();
char read_uart(void);
void write_uart(char);
void init_ring_buffer();
void put_ring_buffer(char);
char pop_ring_buffer();

#endif
