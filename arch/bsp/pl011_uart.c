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

struct uart_ring_buffer{
    unsigned int head;
    unsigned int tail;
    bool full;
    char buffer[UART_INPUT_BUFFER_SIZE];
};

static volatile
struct uart_regs* const regs = (struct uart_regs *) UART_BASE;

static volatile
struct uart_ring_buffer buff;

void init_ring_buffer(){
    buff.head = 0;
    buff.tail = 0;
    buff.full = false;
}

void put_ring_buffer(char character){
    if(!buff.full){
        buff.buffer[buff.head] = character;
        buff.head = (buff.head + 1)%UART_INPUT_BUFFER_SIZE;
        if(buff.head == buff.tail){
            buff.full = true;
        }
    }
}

char pop_ring_buffer(){
    if(buff.tail == buff.head && !buff.full){
        return 0;
    }
    char pop = buff.buffer[buff.tail];
    buff.tail = (buff.tail + 1)%UART_INPUT_BUFFER_SIZE;
    buff.full = false;
    return pop;
}

void setup_int_uart(){
    regs->imsc = 1 << 4;
    init_ring_buffer();
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
