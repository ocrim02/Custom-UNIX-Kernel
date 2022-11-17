#include <lib/ringbuffer.h>

//Struct für den uart RingBuffer

void init_ubuff(uart_buffer* ub){
	ub->write_pos = 0;
	ub->read_pos = 0;
	
	ub->ubuff_full = false;
	ub->ubuff_empty = true;
}

//schreibt Symbol an die aktuelle Stelle von write in den Ringbuffer (wenn nicht voll) und bewegt write eins weiter
//wenn Buffer voll dann false
bool write_ubuff(uart_buffer* ub, char symbol){
	 
	if ((ub->write_pos != ub->read_pos) || (ub->ubuff_empty)){
		
		ub->ubuff_full = false;
		ub->uart_buff[ub->write_pos] = symbol;
		ub->ubuff_empty = false;

		ub->write_pos = ((ub->write_pos)+1) % (UART_INPUT_BUFFER_SIZE);
		
		return true;
	}
	else {
		ub->ubuff_full = true;
		return false;
	}
}

//ließt Symbol von der akutellen Stelle von read im Ringbuffer (wenn nicht leer) und bewegt read eins weiter
//Wenn Buffer leer denn Return NULL => 0
char read_ubuff(uart_buffer* ub){

	if ((ub->read_pos != ub->write_pos) || (ub->ubuff_full)){
		
		ub->ubuff_empty = false;
		char symbol = ub->uart_buff[ub->read_pos];
		ub->ubuff_full = false;
		
		ub->read_pos = ((ub->read_pos)+1) % (UART_INPUT_BUFFER_SIZE);
		
		return symbol;
	}
	else {
		ub->ubuff_empty = true;
		return 0;
	}
}

