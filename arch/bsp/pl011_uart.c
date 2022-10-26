#include <arch/bsp/pl011_uart.h>
#include <arch/bsp/yellow_led.h>

#define UART_BASE (0x7E201000 - 0x3F000000)
#define FR_OFFSET 0x18
#define DR_OFFSET 0x0
//#define LCRH_OFFSET 0x2c


unsigned int* const flag_reg = (unsigned int *) UART_BASE + FR_OFFSET;
//unsigned int* const lcrh_reg = (unsigned int *) UART_BASE + LCRH_OFFSET;

struct uart_dr{
    unsigned int data : 8;
    unsigned int fe : 1;
    unsigned int pe : 1;
    unsigned int be : 1;
    unsigned int oe : 1;
    unsigned int unused0 : 20;
};

struct uart_fr{
    unsigned int cts : 1;
    unsigned int dsr : 1;
    unsigned int dcd : 1;
    unsigned int busy : 1;
    unsigned int rxfe : 1;
    unsigned int txff : 1;
    unsigned int rxff : 1;
    unsigned int txfe : 1;
    unsigned int ri : 1;
    unsigned int unused0 : 23;
};

static volatile
struct uart_dr* const data_reg = (struct uart_dr *) UART_BASE + DR_OFFSET;

static volatile
struct uart_fr* const fr_reg = (struct uart_fr *) UART_BASE + FR_OFFSET;

/*
TXFE: set to 1 when transmission holding reg is empty
RXFF: set to 1 when receive holding reg is full
TXFF: set to 1 when transmission holding reg is full
RXFE: set to 1 when receive holding reg is empty
BUSY: bussy sending
CTS: 
*/

void read_uart(void)
{
    /*unsigned int data = 0;
    unsigned int mask = 0;

    for (unsigned i=0; i<8; i++){
        mask |= 1 << i;
    }
    */
    while (1)
    {
        //data = *data_reg & mask;
        if(data_reg->data == 0x72)
        {
            light_row();
            write_uart('r');
        }
    }
    
}


void write_uart(char character)
{
    data_reg->data = character;
}