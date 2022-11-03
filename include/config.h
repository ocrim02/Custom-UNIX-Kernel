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
#include <lib/utils.h> 
static inline void test_kprintf(void) {
	/* Nur für Aufgabenblatt 1
	 * Hier Test Funktion für kprintf schreiben und
	 * nach dem vollständigen initialisieren aufrufen
	 */
	kprintf("kprintf_test\n\n");

	int i = 65;
	unsigned int adrr = (unsigned int) &i;
	
	
    kprintf("Test für Aufgabe 2:\n\n");
	
    kprintf("Teste Print ohne Variablen:\n");
    
	kprintf("Print erfolgreich?\n\n");
	
    kprintf("Teste Variable einzeln:\n\n");
	
	kprintf("Teste C-Variable:\n\n");
	
    kprintf("soll: A\n");
    
    kprintf("ist : %c\n\n\n", i);

	kprintf("Teste S-Variable:\n\n");
	
	kprintf("soll: ein String sein mit NULL-terminierung. .\n");
	const char* string = "ein String sein mit NULL-terminierung. .\0";
    kprintf("ist : %s\n\n\n", string);

	kprintf("Teste x-Variable:\n\n");
	
	kprintf("soll: 0\n");
    kprintf("ist : %x\n\n",(unsigned int)0);

	kprintf("soll: c\n");
    kprintf("ist : %x\n\n",(unsigned int)12);

	kprintf("soll: 8fb6e\n");
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
	
	kprintf("soll: 134f_19_310_efe_acdc\n");
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
	kprintf("soll: ein String A8fb6e-1785621*-34782 0x%x, 2mal Prozent\n", adrr4);
	const char* strung = "ein String \0";
	
    kprintf("ist : %s%c%x-%u*%i %p, %%%%\n\n\n\n", strung, q, (unsigned int)588654, (unsigned int)1785621, (int)-34782, &h);
    
    
    
    kprintf("Teste Prozentzeichen ohne Variable:\n\n");
    //FEHLER: soll ?
    //nur bei c ist die weitere ausgabe woanders
    
    kprintf("Teste nur ein Prozentzeichen:\n\n");
    
    kprintf("soll: irgendwas_\n");
    kprintf("ist : irgendwas_%komisches \n\n\n");

    kprintf("Teste Symbole und sonderfälle:\n\n");

    //Fehler
    
    kprintf("%§$/()§=?:""=]#'-_\[/d\\n\n\n\n\n");
    
    
    kprintf("Teste 0 bzw leere strings:\n\n");
    
    kprintf("Teste C-Variable:\n\n");
	
    //FEHLER
    kprintf("soll: irgendwas_\n");
    kprintf("ist : irgendwas_%c\n\n\n", (unsigned int) 0);

	
	kprintf("Teste S-Variable:\n\n");
	
	kprintf("soll: irgendwas__\n");
    kprintf("ist : irgendwas_%s_\n\n\n", (char*) "");
    
    kprintf("soll: irgendwas_ _\n");
    kprintf("ist : irgendwas_%s_\n\n\n", (char*) " ");

	kprintf("Teste x-Variable:\n\n");
	
	kprintf("soll: irgendwas_0\n");
    kprintf("ist : irgendwas_%x\n\n\n", (unsigned int) 0);

	kprintf("Teste I-Variable:\n\n");
	
	kprintf("soll: irgendwas_0\n");
    kprintf("ist : irgendwas_%i\n\n\n", (int) 0);

	kprintf("Teste U-Variable:\n\n");
	
	kprintf("soll: irgendwas_0\n");
    kprintf("ist : irgendwas_%u\n\n\n", (unsigned int) 0);

	kprintf("Teste P-Variable:\n\n");
	
	kprintf("bei %%p geht NULL nicht\n");

    
    kprintf("Teste NULL als arg:\n\n");
    
    kprintf("NULL als arg sorgt für compilier Fehler\n\n\n");
    
	//check ob \0 wirkleich den befehl beendet
	
	kprintf("soll: irgendwas_komisches passiert hie\n");
    kprintf("ist : irgendwas_komisches passiert hie\0r");
    kprintf("\n\n\n");
	
	
    kprintf("Teste große unsigned und signed Werte:\n\n");
    //-2147483648 bis 2147483647 :=> Signed int, 429496729 unsigned int 

	kprintf("Teste I-Variable:\n\n");
	
	kprintf("soll: irgendwas_-2147483648\n");
    kprintf("ist : irgendwas_%i\n\n\n", (int) -2147483648);
	
	kprintf("Teste U-Variable:\n\n");
	
	kprintf("soll: irgendwas_429496729\n");
    kprintf("ist : irgendwas_%u\n\n\n\n", (unsigned int) 429496729);
    
	
	
	kprintf("Ende Test Aufgabe 2 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    kprintf("Test für Aufgabe 3:\n\n");
    kprintf("Teste Feldbreite 8 linksseitig mit Leerzeichen aufgefüllt:\n\n");
    
    
	kprintf("Teste x-Variable:\n\n");
	kprintf("soll: Hi ich bin   bee76.\n"); 
	kprintf("ist : Hi ich bin%8x.\n\n", (unsigned int) 781942);

	kprintf("Teste i-Variable:\n\n");
	kprintf("soll: *  -34789*\n");
	kprintf("ist : *%8i*\n\n\n",(int) -34789); 

	kprintf("Teste u-Variable:\n\n");
	kprintf("soll: &&      21&&\n");
	kprintf("ist : &&%8u&&\n\n",(unsigned int) 21);

	kprintf("Teste p-Variable:\n\n");
	kprintf("soll: Adresse: 0x%x() (selbst die Stellen zählen)\n", adrr);
	kprintf("ist : Adresse:%8p()\n\n", &i);
    
    
    
    kprintf("Teste Verhalten bei Feldbreite über 8:\n\n");
    
    kprintf("Sollten an sich alle abrechnen oder abschneiden\n"); 
    
    kprintf("Teste x-Variable:\n\n");
	kprintf("soll: Hi ich bin,67ee76df.\n"); 
	kprintf("ist : Hi ich bin,%8x.\n\n", (unsigned int) 48988321503); //zugroßes unsigned ?

	kprintf("Teste i-Variable:\n\n");
	kprintf("soll: *\n");
	kprintf("ist : *%8i*\n\n\n",(int) -34789887);

	kprintf("Teste u-Variable:\n\n");
	kprintf("soll: &&\n");
	kprintf("ist : &&%8u&&\n\n",(unsigned int) 563367821);

	kprintf("Teste p-Variable:\n\n");
	kprintf("soll: Adresse:0x%x() (nur gültig wenn %%x geht selbst die Stellen zählen)\n", adrr);
	kprintf("ist : Adresse:%8p()\n\n", &i);



    kprintf("Ende Test Aufgabe 3 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    kprintf("Test für Aufgabe 4:\n\n");
    kprintf("Teste Feldbreite 8 linksseitig mit 0en aufgefüllt:\n\n");
         

	kprintf("Teste x-Variable:\n\n");
	kprintf("soll: Hi ich bin000bee76.\n"); 
	kprintf("ist : Hi ich bin%08x.\n\n", (unsigned int) 781942);

	kprintf("Teste i-Variable:\n\n");
	kprintf("soll: *-0034789*\n");
	kprintf("ist : *%08i*\n\n\n",(int) -34789);

	kprintf("Teste u-Variable:\n\n");
	kprintf("soll: &&00000021&&\n");
	kprintf("ist : &&%08u&&\n\n",(unsigned int) 21);

	kprintf("Teste u-Variable:\n\n");
	kprintf("soll: &&00000000&&\n");
	kprintf("ist : &&%08u&&\n\n",(unsigned int) 0);
    
    kprintf("Teste Verhalten bei Feldbreite über 8:\n\n"); //negative und positive Zahlen (kann das minus verschwinden ?)
   	
   	kprintf("Sollten an sich alle abrechnen oder abschneiden\n");
    
	kprintf("Teste x-Variable:\n\n");
	kprintf("soll: Hi ich bin,67ee76df.\n"); 
	kprintf("ist : Hi ich bin,%8x.\n\n", (unsigned int) 48988321503); 

	kprintf("Teste i-Variable:\n\n");
	kprintf("soll: *\n");
	kprintf("ist : *%8i*\n\n\n",(int) -34789887);

	kprintf("Teste u-Variable:\n\n");
	kprintf("soll: &&\n");
	kprintf("ist : &&%8u&&\n\n",(unsigned int) 563367821);

    kprintf("Ende Test Aufgabe 4 ------------------------------------------------------------------------------:\n\n\n\n\n");
    
    //Befehl %06 und so testen ? 
    kprintf("Teste Befehl %%06 unf %%7:\n\n");
	kprintf("ist : &&%06u&&\n\n",(unsigned int) 5633821);
	kprintf("ist : &&%7u&&\n\n",(unsigned int) 5633851);
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
