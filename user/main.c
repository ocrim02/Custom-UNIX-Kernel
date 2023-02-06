#include <user/main.h>

#define PRINT_COUNT 11

volatile unsigned int global_counter = 100;
volatile char letter;

void worker_thread(void* arg){
	char id = *((char*) arg);
	unsigned int priv_counter = 0;
	char string[8];
	int len = uint_to_dec_str(string, global_counter);
	for(int i=0; i<len; i++){
		syscall_putc(string[i]);
	}

	while(global_counter < 130){
		priv_counter++;
		global_counter++;

		syscall_putc(letter);
		syscall_putc(':');
		int len = uint_to_dec_str(string, global_counter);
		for(int i=0; i<len; i++){
			syscall_putc(string[i]);
		}
		syscall_putc(' ');
		syscall_putc('(');
		syscall_putc(id);
		syscall_putc(':');
		len = uint_to_dec_str(string, priv_counter);
		for(int i=0; i<len; i++){
			syscall_putc(string[i]);
		}
		syscall_putc(')');
		syscall_putc('\n');

		syscall_sleep(1);
	}
	syscall_exit();
}

void worker_process(void* arg){
	letter = *((char*) arg);
	syscall_putc(letter);
	char id = '1';
	syscall_thread_create(worker_thread, &id, sizeof(id));
	id = '2';
	syscall_thread_create(worker_thread, &id, sizeof(id));
	id = '3';
	syscall_thread_create(worker_thread, &id, sizeof(id));
	syscall_exit();

}

void main(){
	for(;;){
		char c = syscall_getc();
		syscall_putc(c);
		syscall_process_create(worker_process, &c, sizeof(c));
	}
}


int uint_to_dec_str (char* string, unsigned int value)
{
    //catch value = 0
    if(value == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return 2;
    }

    unsigned int divisor = 1;
    int len = 1;
    while(value/(divisor*10) != 0){
        len++;
        divisor = divisor * 10;
        if(divisor == 1000000000){
            break;
        }
    }
    
    int digit;
    for(int i=0; i<=len-1; i++)
    {
        digit = value/divisor;
        string[i] = digit + '0';
        value = value - digit * divisor;
        divisor = divisor/10;
    }
    string[len] = '\0'; 

    return len;
}
