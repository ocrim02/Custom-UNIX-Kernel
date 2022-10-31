#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Falls auf den Boards gearbeitet wird am besten
 * die nächste Zeile auskommentieren
 */
#define __QEMU__

/**
 * \file config.h
 *
 * Enthält defines und static Funktionen zum testen der
 * Implementierung. Wir tauschen diese Werte/Funktionen beim
 * Korrigieren zum Testen aus. Bitte fügt hier KEINE weiteren
 * defines oÄ ein. Ihr könnt diese Werte zum Testen natürlich
 * auch gerne selbst verändern.
 */

/* Include-Pfad bitte gegenfalls anpassen */
//#include <lib/utils.h> //muss kprintf als datei erstellt werden ?
static inline void test_kprintf(void) {
	/* Nur für Aufgabenblatt 1
	 * Hier Test Funktion für kprintf schreiben und
	 * nach dem vollständigen initialisieren aufrufen
	 */
	kprintf("kprintf_test\n\n");
	
    kprintf("Test für Aufgabe 2:\n\n");
	
    kprintf("Teste Print ohne Variablen:\n\n");
    
	kprintf("Print erfolgreich?\n\n\n");
	
    kprintf("Teste Variable einzeln:\n\n");
	
	kprintf("Teste C-Variable:\n\n");
	
    kprintf("soll: A\n");
    int i = 65;
    kprintf("ist : %c\n\n\n", i);

	kprintf("Teste S-Variable:\n\n");
	
	kprintf("soll: ein String sein mit NULL-terminierung. .\n");
	const char* string = "ein String sein mit NULL-terminierung. .\0";
    kprintf("ist : %s\n\n\n", string);

	kprintf("Teste x-Variable:\n\n");
	
	kprintf("soll: 0\n");
    kprintf("ist : %x\n\n",(unsigned int)0);

	kprintf("soll: C\n");
    kprintf("ist : %x\n\n",(unsigned int)12);

	kprintf("soll: 8FB6E\n");
    kprintf("ist : %x\n\n\n",(unsigned int)588654);

	kprintf("Teste I-Variable:\n\n");
	
	kprintf("soll: 0\n");
    kprintf("ist : %i\n\n",(unsigned int)0);

	kprintf("soll: 12\n");
    kprintf("ist : %i\n\n",(int)12);

	kprintf("soll: -347825612\n");
    kprintf("ist : %i\n\n\n",(int)-347825612);

	kprintf("Teste U-Variable:\n\n");
	
	kprintf("soll: 1785621\n");
    kprintf("ist : %u\n\n",(unsigned int)1785621);

	kprintf("soll: 4293181675\n");
    kprintf("ist : %u\n\n",(unsigned int)-1785621);

	kprintf("Teste P-Variable:\n\n"); //??????????????
	
	unsigned int adrr = (unsigned int) &i;
	
	kprintf("soll: 0x%x (nur gültig wenn %%x geht)\n", adrr);
    kprintf("ist : %p\n\n", &i);

	kprintf("Teste Prozentzeichen:\n\n");
	
	kprintf("soll: Prozentzeichen\n");
    kprintf("ist : %%\n\n");

	kprintf("soll: Prozentzeichen und Text\n");
    kprintf("ist : %% und Text\n\n\n\n");
    

    kprintf("Teste mehrfach gleiche Variable:\n\n");
    
    kprintf("Teste C-Variable:\n\n");
    
    kprintf("soll: HALLO\n");
    int h = 72; 
    int a = 65;
    int l = 76;
    int o = 79;
    kprintf("ist : %c%c%c%c%c\n\n\n", h, a, l, l, o);

	kprintf("Teste S-Variable:\n\n");
	
	kprintf("soll: Drei_kleine_strings.\n");
	const char* drei = "Drei_\0";
	const char* kleine = "kleine_\0";
	const char* string2 = "strings.\0";
    kprintf("ist : %s%s%s\n\n\n", drei, kleine, string2);

	kprintf("Teste x-Variable:\n\n");
	
	kprintf("soll: 134F_19_310_EFE_ACDC\n");
	unsigned int hex_1 = 4943;
	unsigned int hex_2 = 25;
	unsigned int hex_3 = 784;
	unsigned int hex_4 = 3838;
	unsigned int hex_5 = 44252;
    kprintf("ist : %x_%x_%x_%x_%x\n\n\n", hex_1, hex_2, hex_3, hex_4, hex_5);


	kprintf("Teste I-Variable:\n\n");

	kprintf("soll: 36.-238\n");
	int x = 36;
	int y = -238;
    kprintf("ist : %i.%i\n\n\n", x, y);


	kprintf("Teste U-Variable:\n\n");
	
	kprintf("soll: 4293181675-35-234\n");
	unsigned int u1 = -1785621;
	unsigned int u2 = 35;
	unsigned int u3 = 234;
    kprintf("ist : %u-%u-%u\n\n\n", u1, u2, u3);

	kprintf("Teste P-Variable:\n\n");
	
	unsigned int adrr1 = (unsigned int) &u1;
	unsigned int adrr2 = (unsigned int) &u2;
	unsigned int adrr3 = (unsigned int) &u3;
	
	kprintf("soll: 0x%x*0x%x*0x%x (nur gültig wenn %%x geht)\n", adrr1, adrr2, adrr3);
	void* v1 = &u1;
	void* v2 = &u2;
	void* v3 = &u3;
    kprintf("ist : %p*%p*%p\n\n\n", v1, v2, v3);

	kprintf("Teste Prozentzeichen:\n\n");
	

	kprintf("soll: 3 Prozentzeichen zwichen Leerzeichen\n");
    kprintf("ist : %% %% %% \n\n");
	

	kprintf("soll: 3 Prozentzeichen ohne Leerzeichen\n");
    kprintf("ist : %%%%%% \n\n\n\n");


    kprintf("Teste mehrere Variablen:\n\n");
    
	int q = 65;
	unsigned int adrr4 = (unsigned int) &h;
	kprintf("soll: ein String A8FB6E-1785621*-34782 0x%x, 2mal Prozent\n", adrr4);
	const char* strung = "ein String \0";
	
    kprintf("ist : %s%c%x-%u*%i %p, %%%%\n\n\n\n", strung, q, (unsigned int)588654, (unsigned int)1785621, (int)-34782, &h);
    
    //FEHLER: ohne , &h) gibt &p 0x0 aus 


    kprintf("Teste Prozentzeichen ohne Variable:\n\n");
    //% ohne Arg =>Fehler 
    // mit oder ohne komma? [...", variable) oder ...")]
    
    //%p zeigt bei keinem argument 0x
    kprintf("Teste nur ein Prozentzeichen:\n\n");
    
    kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : % \n\n\n");
    
    kprintf("Teste C-Variable:\n\n");
	
    kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %c\n\n\n");

	kprintf("Teste S-Variable:\n\n");
	
	kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %s\n\n\n");

	kprintf("Teste x-Variable:\n\n");
	
	kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %x\n\n\n");

	kprintf("Teste I-Variable:\n\n");
	
	kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %i\n\n\n");

	kprintf("Teste U-Variable:\n\n");
	
	kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %u\n\n\n");

	kprintf("Teste P-Variable:\n\n");
	
	kprintf("soll: nichts oder Fehler\n");
    kprintf("ist : %p\n\n\n\n");
	
	//fehler manche nehmen das letzte argument wieder
        
    kprintf("Teste Variablen mit unzureichend vielen argumenten:\n\n");
    //3->2
    //2->1

	kprintf("Teste Verhalten bei Variablen mit falschen Argumenten:\n\n");
	
    //Bei i und u Test Werte nehmen die die anderen nicht 	nehmen können ? Werte Bereich und negatives ?
/*
	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %c\n\n\n", i);

	kprintf("Teste S-Variable:\n\n");

	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %s\n\n\n", string);

	kprintf("Teste x-Variable:\n\n");

	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %x\n\n\n",(unsigned int)588654);

	kprintf("Teste I-Variable:\n\n"); 

	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %i\n\n\n",(int)-347825612);

	kprintf("Teste U-Variable:\n\n");

	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %u\n\n",(unsigned int)17856);

	kprintf("Teste P-Variable:\n\n");

	unsigned int adrr = (unsigned int) &i;
	kprintf("soll: Fehler oder nichts\n");
	kprintf("ist : %p\n\n\n\n", &i);
*/

    kprintf("Teste Symbole und sonderfälle:\n\n");

    //teste symbole zb ""
    //ausgabe von nichts leere strings und zeug
    
    //mit variablen übertreiben
	//text mit variablen 
	//check ob \0 den befehl beendet
	
	/*Bsprak mehr cases 
0 cases und leere Strings

Variablen wie im Einzel Case benutzen für ein Argument und dann alle variablen testen mit zu wenig Argumenten

Bei falschen variablen wie bei einzeltest nur mit falsch gecarsteten args
	*/

	//notes sektion machen
	/* -2147483648 
 	2147483647
	Signed int
	4294967295
unsigned int */
	

	kprintf("Ende Test Aufgabe 2 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    kprintf("Test für Aufgabe 3:\n\n");
    kprintf("Teste Feldbreite 8 linksseitig mit Leerzeichen aufgefüllt:\n\n");
    	//negative und positive Zahlen
    	//nur i u x p
    kprintf("Teste Verhalten bei Feldbreite über 8:\n\n");

    kprintf("Ende Test Aufgabe 3 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    kprintf("Test für Aufgabe 4:\n\n");
    kprintf("Teste Feldbreite 8 linksseitig mit 0en aufgefüllt:\n\n");
        //negative und positive Zahlen
        //nur i u x 
    kprintf("ist : hallo %%0 ERROR expected %%08!\n");
    kprintf("ist : hallo %06 \n\n");
    
    kprintf("Teste Verhalten bei Feldbreite über 8:\n\n");
        //negative und positive Zahlen (kann das minus verschwinden ?)

    kprintf("Ende Test Aufgabe 4 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    //kprintf einzeilig mit \n trennen und aufrufen 
    //zb kprintf(" soll: 3 Prozentzeichen zwichen Leerzeichen\n ist : %% %% %% \n\n");
    
}

/**
 * Erst ab Aufgabenblatt 2 relevant
 */

#ifdef __QEMU__
/* Werte zum testen unter QEMU */
#define BUSY_WAIT_COUNTER 10000000
#else
/* Werte zum testen auf der Hardware */
#define BUSY_WAIT_COUNTER 30000
#endif // __QEMU__

// Wir testen nur mit Werten die durch 2^n darstellbar sind
#define UART_INPUT_BUFFER_SIZE 128

// Timer Interrupt Interval zum testen in Mikrosekunden
// Systimer taktet mit 1MHz
// 1000000 -> 1 Sekunde
#define TIMER_INTERVAL 1000000


#endif // _CONFIG_H_
