#include <user/main.h>

#define PRINT_COUNT 11

void worker_thread(void* arg){
	char c = *((char *) arg);

	switch (c){
	case 'n':
		//read null
		read_addr(0);
		break;
	case 'p':
		//jump null
		jump_addr(0);
		break;
	case 'd':
		//read kernel_data
		read_addr((unsigned int) kernel_data_section);
		break;
	case 'k':
		//read kernel_code
		read_addr((unsigned int) kernel_text_section);
		break;
	case 'K':
		//read kernel_stack
		read_addr(128*1024*1024 - 4);
		break;
	case 'g':
		//read uart
		read_addr(0x7E201000 - 0x3F000000);	
		break;
	case 'c':
		//write own code
		write_addr((unsigned int) user_text_section);
		break;
	case 's':
		//stack-overflow
		stack_overflow();
		break;
	case 'u':
		//read unassigned addr
		read_addr((unsigned int) user_bss_section + ONE_MB);
		break;
	case 'x':
		//jump own data
		jump_addr((unsigned int) user_data_section);
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
