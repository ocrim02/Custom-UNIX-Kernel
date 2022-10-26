#ifndef PL011_UART_H
#define PL011_UART_H

void read_uart(void);
void write_uart(char);
void kprintf(char*, ...);
char* int_to_str(int);
char* unsigned_int_to_str(unsigned int);

#endif
