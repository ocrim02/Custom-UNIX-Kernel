
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
#include <arch/cpu/ivt.h>

volatile unsigned int counter = 0;

void increment_counter() {
	counter++;
}


void start_kernel(){
	set_ivt();
	interrupt_setup();

	while(1){
		char character = read_uart();
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal %u\n", character, (unsigned int) character, (unsigned int) character);
		volatile unsigned int* test = (unsigned int*) 0x1; 
		volatile unsigned int d;
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
	}

	}
	

	// Endless counter
	for (;;) {
		increment_counter();
	}

}
