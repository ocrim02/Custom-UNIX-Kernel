#ifndef PL011_UART_H
#define PL011_UART_H

#include <stdbool.h>
#include <config.h>
#include <lib/utils.h>
/*

#include <arch/cpu/interrupt_handler.h>
#include <arch/cpu/interrupt_controller.h>
*/



void read_uart(void);
void write_uart(char);

#endif