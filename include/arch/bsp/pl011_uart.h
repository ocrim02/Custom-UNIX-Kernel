#ifndef PL011_UART_H
#define PL011_UART_H

void read_uart(void);
void write_uart(char);
void kprintf(char*, ...);
char* int_to_dec_str(int);
char* unsigned_int_to_dec_str(unsigned int);
char* unsigned_int_to_hex_str(unsigned int);
char* void_to_hex_str(void*);

#endif
