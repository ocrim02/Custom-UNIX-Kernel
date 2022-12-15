
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
#include <kernel/thread.h>

volatile unsigned int counter = 0;


void increment_counter() {
	counter++;
}


void start_kernel(){
	init_threads();
	set_ivt();
	interrupt_setup();
	//switch_loop_mode();
	increment_compare(TIMER_INTERVAL, C1);
	setup_int_uart();

	thread_create(&main, 0, 0);

	// Endless counter
	for (;;) {
		increment_counter();
	}

}


