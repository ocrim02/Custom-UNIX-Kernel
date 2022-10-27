#include <arch/bsp/pl011_uart.h>
#include <arch/bsp/yellow_led.h>
#include <stdarg.h>
#include <stdbool.h>

#define UART_BASE (0x7E201000 - 0x3F000000)
#define FR_OFFSET 0x18
#define DR_OFFSET 0x0

#define INT_SIZE 32
#define UNSIGNED_INT_SIZE 32
#define VOID_SIZE 32
#define MAX_INT_STRING_SIZE 11
#define MAX_UNSIGNED_INT_STRING_SIZE 10
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


TODO:
- comments
- error returns
- check int and unsigned int sizes

*/

//implement FR checks
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
            //kprintf("Hello I am %i Years old. %u lol\n", -2100000000, 2200000000);
            kprintf("int: %i.\n",-2147483647);
            kprintf("uint: %u.\n",(unsigned int)2147483648);
            kprintf("The right address is %x.\n", (unsigned int) 4294967295);
            kprintf("The right address is %u.\n", (unsigned int) 4294967293);
            kprintf("The right address is %p.\n", (void*) 0xFFFFFFFF);
            
            //int_to_str(327);
        }
    }
    
}

//implement FR checks
void write_uart(char character)
{
    data_reg->data = character;
}

void kprintf(char* string, ...)
{
    
    va_list ap;
    va_start(ap, string);
    bool spaces = false;
    bool zeros = false;
    
    char character = *string++;
    while(character != '\0'){
        if(character == '%')
        {
            character = *string++;
            /*if(character == '8'){
                spaces = true;
                character = *string++;
            }
            else if(character == '0'){
                if(*string++ != '8'){
                    //error
                }
                zeros = true;
            }*/

            if(character == 'c'){
                int value = va_arg(ap, int);
                write_uart((unsigned char) value);
                character = *string++;
            }
            else if(character == 's'){
                char* value = va_arg(ap, char*);
                kprintf(value);
                character = *string++;
            }
            else if(character == 'x'){
                unsigned int value = va_arg(ap, unsigned int);
                kprintf(unsigned_int_to_hex_str(value));
                character = *string++;
            }
            else if(character == 'i'){
                int value = va_arg(ap, int);
                kprintf(int_to_dec_str(value));
                character = *string++;
            }
            else if(character == 'u'){
                unsigned int value = va_arg(ap, unsigned int);
                kprintf(unsigned_int_to_dec_str(value));
                character = *string++;
            }
            else if(character == 'p'){
                void* value = va_arg(ap, void*);
                kprintf(void_to_hex_str(value));
                character = *string++;
            }
            
            
        }
        write_uart(character);
        character = *string++;
    }
    va_end(ap);
}

//converts unsigned int to char array
char* unsigned_int_to_dec_str (unsigned int value)
{
    static char string[MAX_UNSIGNED_INT_STRING_SIZE + 1];

    //catch value = 0
    if(value == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return string;
    }

    unsigned int divisor = 1;
    int len = 1;
    while(value/(divisor*10) != 0){
        len++;
        divisor = divisor * 10;
        if(divisor == 1000000000){
            break;
        }
    }
    
    
    int digit;
    for(int i=0; i<=len-1; i++)
    {
        digit = value/divisor;
        string[i] = digit + '0';
        value = value - digit * divisor;
        divisor = divisor/10;
    }
    string[len] = '\0'; 

    return string;
}

//converts int to char array
char* int_to_dec_str (int value)
{
    static char string[MAX_INT_STRING_SIZE + 1];
    int negatives_offset = 0;
    if(value < 0){
        string[0] = '-';
        negatives_offset = 1;
        value = value * (-1);
    }

    char* rest = unsigned_int_to_dec_str((unsigned int) value);
    for(int i=0; i<= MAX_UNSIGNED_INT_STRING_SIZE; i++){
        string[i + negatives_offset] = rest[i];
    }

    return string;
    

}

char* unsigned_int_to_hex_str(unsigned int value)
{
    int digit_count = UNSIGNED_INT_SIZE/4;
    unsigned int digit_values[UNSIGNED_INT_SIZE/4];
    for(int i=0; i<digit_count; i++){
        digit_values[i] = 0;
    }
    unsigned int mask = 15; //...0000000000001111
    for(int i=digit_count-1; i>=0; i--){
        digit_values[i] = value & mask;
        value = value >> 4;
    }

    //conversion list
    char conversion_list[16] = "0123456789ABCDEF";
    static char string[(UNSIGNED_INT_SIZE/4)+1];

    int offset = 0;
    for(int i=0; i<digit_count; i++){
        if(digit_values[i] != 0){
            break;
        }
        else{
            offset++;
        }
    }
    for(int i=0; i<digit_count-offset; i++){
        string[i] = conversion_list[digit_values[i+offset]];
    }
    string[digit_count-offset] = '\0';

    return string;
}

char* void_to_hex_str(void* pointer)
{
    static char string[(UNSIGNED_INT_SIZE/4)+3];
    string[0] = '0';
    string[1] = 'x';
    char* content = unsigned_int_to_hex_str((unsigned int) pointer);
    for(int i=0; i<(UNSIGNED_INT_SIZE/4); i++){
        string[i+2] = content[i];
    }
    return string;
}