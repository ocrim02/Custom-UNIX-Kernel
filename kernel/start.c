
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

#include <config.h>

/*
#include <arch/bsp/pl011_uart.h>
#include <lib/ringbuffer.h>
#include <arch/cpu/ivt.h>
#include <arch/cpu/interrupt_handler.h>
#include <arch/cpu/interrupt_controller.h>
*/

volatile unsigned int counter = 0;

void increment_counter() {
	counter++;
}



void start_kernel(){

	set_ivt();

	enable_irq(EN_ARM_TIMER_BASIC, EN_SYSTIMER_C1_EN1, EN_SYSTIMER_UART_EN2);
	init_systimer(1, TIMER_INTERVAL);
	
	//print_uart_regs();
	read_uart();

	// Endless counter
	for (;;) {
		increment_counter();
	}
}