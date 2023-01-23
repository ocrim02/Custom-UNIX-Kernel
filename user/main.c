#include <user/main.h>

#define PRINT_COUNT 11

void worker_thread(void* arg){
	char c = *((char *) arg);

	switch (c){
	case 'n':
		//read null
		break;
	case 'p':
		//jump null
		break;
	case 'd':
		//read kernel_data
		break;
	case 'k':
		//read kernel_code
		break;
	case 'K':
		//read kernel_stack
		break;
	case 'g':
		//read uart
		break;
	case 'c':
		//write own code
		break;
	case 's':
		//stack-overflow
		break;
	case 'u':
		//read unassigned addr
		break;
	case 'x':
		//jump own data
		break;
	default:
		break;
	}

	for(unsigned int i=0; i<PRINT_COUNT; i++){
		syscall_putc(c);

		if( c >= 'A' && c <= 'Z'){
			for(unsigned int a=0; a<BUSY_WAIT_COUNTER*40; a++){}
		}
		else{
			syscall_sleep(2);
		}
	}
	syscall_exit();
}

void main(){
	for(;;){
		char c = syscall_getc();
		syscall_thread_create(worker_thread, &c, sizeof(c));
	}
}
