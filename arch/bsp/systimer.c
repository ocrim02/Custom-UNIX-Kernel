#include <arch/bsp/systimer.h>

#define SYSTTIMER_BASE (0x7E003000 - 0x3F000000)

//unsigned int = 4bytes = 32 bit

struct systim_regs{
    unsigned int cs;
    unsigned int clo;
    unsigned int chi;
    unsigned int c0;
    unsigned int c1;
    unsigned int c2;
    unsigned int c3;
};

static volatile
struct systim_regs* const regs = (struct systim_regs *) SYSTTIMER_BASE;


//timer macht es von alleine 
//zieht einen interrupt => kern merkt es im interrupt schmeißt IRQ und es muss da in der IVT =>IRQ behandelt werden

//interrupt wieder clearen nach der letzten fkt
//so lange wird an der interrupt line gezogen

//interupt controller configurieren VL2 S.26
//zum aufteilen der interrupt lines

//c0 und 2 sind für gpu reserviert es sollen nur c1 und 3 verwendet werden

//setzt den timer c1
void init_systimer_c1(unsigned int timer) 
{
    regs->c1 = timer;
    return;
}
//init c 1 2 3 machen wenn 0 geht 

//findet heraus welcher timer den interrupt erzeugt hat
//soll aufgerufen werden vom interrupt service bzw controller 
void timer_irq_solver(void)
{       
        unsigned int status = regs->cs;
		
		//check welcher timer ausgelößt dann gehen
        if(status & (1<<0)){
            //schreibt bit (m0) zum timer interrupt clear
        	regs->cs |= (1<<0);
        
        	//timerwert erhöhen für neuen interrupt 
        	//vorrausgesetzt t_regs->c0 wurde nicht zurückgesetzt
        	regs->c0 += regs->c0;
        }
        /* erst wenn erstes geht
        else if(status & (1<<1)){
            //schreibt bit (m0) zum timer interrupt clear
        	regs->cs |= (1<<1);
        
        	//timerwert erhöhen für neuen interrupt 
        	regs->c1 += regs->c1;
        }
        else if(status & (1<<2)){
            //schreibt bit (m0) zum timer interrupt clear
        	regs->cs |= (1<<2);
        
        	//timerwert erhöhen für neuen interrupt 
        	regs->c2 += regs->c2;
        }
        else if(status & (1<<3)){
            //schreibt bit (m0) zum timer interrupt clear
        	regs->cs |= (1<<3);
        
        	//timerwert erhöhen für neuen interrupt 
        	regs->c3 += regs->c3;
        }
        */
        //mit true wert wenn einer gefunden wurde in der if abrage ?
        return;
}
