
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

#include <arch/bsp/yellow_led.h>
#include <arch/bsp/pl011_uart.h>


volatile unsigned int counter = 0;

void increment_counter() {
	counter++;
}






void start_kernel(){

	read_uart();

	// Endless counter
	for (;;) {
		increment_counter();
	}

}
