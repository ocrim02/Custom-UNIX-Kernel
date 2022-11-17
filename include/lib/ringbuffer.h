#include <config.h>

#include <stdarg.h>
#include <stdbool.h>

typedef struct {
	char uart_buff[UART_INPUT_BUFFER_SIZE];

	int write_pos;
	int read_pos;

	bool ubuff_full; 
	bool ubuff_empty; 
} uart_buffer;


void init_ubuff(uart_buffer* ub);
bool write_ubuff(uart_buffer* ub, char symbol);
char read_ubuff(uart_buffer* ub);
