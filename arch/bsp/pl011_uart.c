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
TODO:
- to few or to much args -> mirco
- comments -> stefan
- error/function returns -> mirco
- tests -> stefan
- trennung von funktion und treiber -> mirco
*/

//implement FR checks
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
                kprintf("test: %8i\n", -123456789); //error
                kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal %u\n", character, (unsigned int) character, (unsigned int) character);
            }
        }
    }
    
}

//implement FR checks
void write_uart(char character)
{
    //loop until not busy
    while(read_masked(regs->fr, 3, 3) != 0){}

    regs->data = write_masked(0, (unsigned int) character, 7, 0);

    return;
}