#include <user/main.h>

#define PRINT_COUNT 20

void main(void* args){
	char c = *((char*) args);

	switch(c){
		case 'a':
			create_data_abort();
			return;
		case 'p':
			create_prefetch_abort();
			return;
		case 'u':
			create_undefined_instruction();
			return;
		case 's':
			create_supervisor_call();
			return;
		case 'r':
			register_checker();
			return;
	}

	for(unsigned int n=0; n<PRINT_COUNT; n++){
		volatile unsigned int i = 0;
		for(; i<(BUSY_WAIT_COUNTER); i++){}
		kprintf("%c", c);
	}

	return;
}
