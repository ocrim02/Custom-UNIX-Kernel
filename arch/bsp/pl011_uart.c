#include <arch/bsp/pl011_uart.h>

#define UART_BASE (0x7E201000 - 0x3F000000)


struct uart_regs{
    unsigned int data;
    unsigned int unused0[5];
    unsigned int fr;
    unsigned int unused1[8];
    unsigned int ris;
};

static volatile
struct uart_regs* const regs = (struct uart_regs *) UART_BASE;

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
            if(character != 0x0){
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
            /*
			case 'e':
				switch_loop_mode();
				charcter_loop();
				break;
				*/
			}
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
