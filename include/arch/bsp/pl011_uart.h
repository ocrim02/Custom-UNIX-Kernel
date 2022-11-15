#ifndef PL011_UART_H
#define PL011_UART_H

#include <stdbool.h>
#include <config.h>
#include <lib/utils.h>

void setup_int_uart();
void get_pendings_uart();
char read_uart(void);
void write_uart(char);

#endif
