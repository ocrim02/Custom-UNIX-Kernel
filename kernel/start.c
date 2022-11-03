
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

volatile unsigned int counter = 0;

void increment_counter() {
	counter++;
}



void start_kernel(){

	test_kprintf();
	read_uart();

	// Endless counter
	for (;;) {
		increment_counter();
	}

}
