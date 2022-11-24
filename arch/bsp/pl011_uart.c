#include <arch/bsp/pl011_uart.h>

#define UART_BASE (0x7E201000 - 0x3F000000)


struct uart_regs{
    unsigned int data;
    unsigned int unused0[5];
    unsigned int fr;
    unsigned int ilpr;
    unsigned int ibrd;
    unsigned int fbrd;
    unsigned int lcrh;
    unsigned int cr;
    unsigned int ifls;
    unsigned int imsc;
    unsigned int ris;
    unsigned int mis;
    unsigned int icr;
    unsigned int dmacr;
    unsigned int itcr;
    unsigned int itio;
    unsigned int itop;
    unsigned int tdr;
};
//fifo geht nicht aus wenn an ist reg ausschalten
//text unter cr angucken--


static volatile
struct uart_regs* const regs = (struct uart_regs *) UART_BASE;


bool loop_mode = false;

/*
uart_buffer ubuff;
uart_buffer* ubuinit = &ubuff;
init_ubuff(ubuinit);
*/

void switch_loop_mode(){
    if(loop_mode){
	    loop_mode = false;
    }
    else{
        loop_mode  = true;
   }
}    

void set_uart_interrupt(){
    regs->imsc = 1<<4;
    uart_buffer ubuffinit;
	uart_buffer* ubuinit = &ubuffinit;
	init_ubuff(ubuinit);
}



/*
 * read a single char from uart 
 * gives a response via uart
 */
void read_uart(void)
{
    //test_kprintf();
    unsigned int dr = 0;
    while (1)
    {   
        
        //wait for empty fifo
        while(read_masked(regs->fr, 4, 4) == 1);
        dr = regs->data;

        //Error catches
        if(dr & (1<<11)){
            kprintf("Overrun Error\n");
        }
        else if(dr & (1<<10)){
            kprintf("Break Error\n");
        }
        else if(dr & (1<<9)){
            kprintf("Parity Error\n");
        }
        else if(dr & (1<<8)){
            kprintf("Framing Error\n");
        }
        else{
            char character = (char) read_masked(dr, 7, 0);

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
                switch_character_loop_mode();
				switch_loop_mode(); 
                //ausrufezeichen mit abbruch
				
                
                
                ///////bit mit case ob in buffer oder nicht in buffer geschrieben
                //activierung und deact mit e ? und write ?

                break;

            case 'c':
                //ausführen regchecker
                break;
			}

            //if (loop_mode & loop_output) {
            if (loop_mode && character != 0x0) {

                for (int i = 0; i < 65; i++)
                {
                    kprintf("%c", character);
                }

                //read_uart();
                
                //mit if concatinieren wenn interupt nicht unterbricht
                //return; //und die beiden if caes für loop nach oben packen wenn funktion ausgeschaltet werden soll
            }
            else{
                if(character != 0x0){
                    kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal %u\n", character, (unsigned int) character, (unsigned int) character);
                }
            }
            /*
            while (loop_output) {
                kprintf("%c", toLoop);
            }*/
        }
    }
}


/*
 * writes out character over uart
 */
void write_uart(char character)
{
    //loop until not busy
    while(read_masked(regs->fr, 3, 3) != 0){}

    regs->data = write_masked(0, (unsigned int) character, 7, 0);

    return;
}

void print_uart_regs(void){
    kprintf("DR:    %x\n", (unsigned int) regs->data);
    kprintf("FR:    %x\n", (unsigned int) regs->fr);
    kprintf("ILPR:  %x\n", (unsigned int) regs->ilpr);
    kprintf("IBRD:  %x\n", (unsigned int) regs->ibrd);
    kprintf("FBRD:  %x\n", (unsigned int) regs->fbrd);
    kprintf("LCRH:  %x\n", (unsigned int) regs->lcrh);
    kprintf("CR:    %x\n", (unsigned int) regs->cr);
    kprintf("IFLS:  %x\n", (unsigned int) regs->ifls);
    kprintf("IMSC:  %x\n", (unsigned int) regs->imsc);
    kprintf("RIS:   %x\n", (unsigned int) regs->ris);
    kprintf("MIS:   %x\n", (unsigned int) regs->mis);
    kprintf("ICR:   %x\n", (unsigned int) regs->icr);
    kprintf("DMACR: %x\n", (unsigned int) regs->dmacr);
    kprintf("ITCR:  %x\n", (unsigned int) regs->itcr);
    kprintf("ITIO:  %x\n", (unsigned int) regs->itio);
    kprintf("ITOP:  %x\n", (unsigned int) regs->itop);
    kprintf("TDR:   %x\n", (unsigned int) regs->tdr);
}