
/// Das vorherige Aufgabenblatt wurde als Demonstration wie die
/// binäre Lösung verwendet werden kann implementiert.
/// Die Implementierung darf/soll/muss abgeändert werden um das
/// nächste Aufgabenblatt zu implementieren.
/// Um die Demonstration nicht über zu viele Dateien zu verstreuen
/// ist alles in:
/// - kernel/start.c
/// - arch/cpu/entry.S
/// zu finden.
/// Es bietet sich jedoch an manche Funktionalitäten in andere
/// Dateien aus zu lagern.

#include <arch/bsp/pl011_uart.h>
#include <config.h>
#include <arch/cpu/interrupt.h>
#include <arch/bsp/timer.h>
#include <arch/cpu/ivt.h>
#include <tests/regcheck.h>
#include <arch/cpu/exception_creator.h>

volatile unsigned int counter = 0;


void increment_counter() {
	counter++;
}

void charcter_loop(){
	while(1){
		char character = pop_ring_buffer();
		if(character != 0){
			for(int i=0; i<40; i++){
				kprintf("%c", character);
				busy_wait(BUSY_WAIT_COUNTER);
			}
		}
	}
}


void start_kernel(){
	set_ivt();
	interrupt_setup();
	increment_compare(TIMER_INTERVAL);
	setup_int_uart();
	
	//pendings();
	

	while(1){
		//char character = read_uart();
		char character = pop_ring_buffer();
		if(character != 0){
			kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal %u\n", character, (unsigned int) character, (unsigned int) character);
		}
		switch(character){
			case 'p':
				//prefetch abort
				create_prefetch_abort();
				break;
			case 's':
				//supervisor call
				create_supervisor_call();
				break;
			case 'a':
				//data abort
				create_data_abort();
				break;
			case 'u':
				//undefined instruction
				create_undefined_instruction();
				break;
			case 'd':
				switch_irq_regdump();
				break;
			case 'e':
				switch_loop_mode();
				charcter_loop();
				break;
			case 'c':
				switch_loop_mode();
				register_checker();
				break;
		}

	}

	// Endless counter
	for (;;) {
		increment_counter();
	}

}


