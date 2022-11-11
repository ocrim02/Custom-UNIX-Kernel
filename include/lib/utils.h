#ifndef UTILS_H
#include <arch/bsp/pl011_uart.h>
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>


void kprintf(char*, ...);
char* int_to_dec_str(char*, int);
char* unsigned_int_to_dec_str(char*, unsigned int);
char* unsigned_int_to_hex_str(char*, unsigned int);
char* void_to_hex_str(char*, void*);
char* eight_number(char*, char*, bool, bool);
int str_len(char*);
unsigned int read_masked(unsigned int, int, int);
unsigned int write_masked(unsigned int, unsigned int, int, int);

#endif
