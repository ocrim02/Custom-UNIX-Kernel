#include <arch/bsp/pl011_uart.h>

#define UART_BASE (0x7E201000 - 0x3F000000)


struct uart_regs{
    unsigned int data;
    unsigned int unused0[5];
    unsigned int fr;
    unsigned int unused1[7];
    unsigned int imsc;
    unsigned int ris;
    unsigned int mis;
    unsigned int icr;
};

static volatile
struct uart_regs* const regs = (struct uart_regs *) UART_BASE;


void setup_int_uart(){
    
    regs->imsc = 1 << 4;
    regs->imsc |= 1 << 5;
    kprintf("mask: %x\n", regs->imsc);
}

void get_pendings_uart(){
    kprintf("pending Uart: %x\n", regs->mis);
}

/*
 * read a single char from uart 
 * gives a response via uart
 */
char read_uart(void)
{
    //test_kprintf();
    unsigned int dr = 0;

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
            return character;
        }
    }
    return 0x0;
    
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
