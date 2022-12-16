#include <user/main.h>

#define PRINT_COUNT 11

void worker_thread(void* arg){
	char c = *((char *) arg);

	if(c == 's'){
		undef_syscall();
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
	//syscall_exit();
}

void main(){
	for(;;){
		char c = syscall_getc();
		syscall_thread_create(worker_thread, &c, sizeof(c));
	}
}
