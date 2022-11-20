#ifndef UTILS_H
//#include <arch/bsp/pl011_uart.h>
#define UTILS_H

#include <config.h>
#include <stdarg.h>
#include <stdbool.h>



void kprintf(char*, ...);
char* int_to_dec_str(int);
char* unsigned_int_to_dec_str(unsigned int);
char* unsigned_int_to_hex_str(unsigned int);
char* void_to_hex_str(void*);
char* eight_number(char*, bool, bool);
int str_len(char*);
unsigned int read_masked(unsigned int, int, int);
unsigned int write_masked(unsigned int, unsigned int, int, int);

#endif
