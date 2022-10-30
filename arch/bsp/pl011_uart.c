#include <arch/bsp/pl011_uart.h>

#define UART_BASE (0x7E201000 - 0x3F000000)
#define FR_OFFSET 0x18
#define DR_OFFSET 0x0

#define INT_SIZE 32
#define UNSIGNED_INT_SIZE 32
#define VOID_SIZE 32
#define MAX_INT_STRING_SIZE 11
#define MAX_UNSIGNED_INT_STRING_SIZE 10

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
- comments
- error returns
- check int and unsigned int sizes -> 32 bit
*/

//implement FR checks
void read_uart(void)
{
    unsigned int dr = 0;
    while (1)
    {
        //wait for empty fifo
        while(read_masked(regs->fr, 4, 4) == 1);
        dr = regs->data;

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
        }
    }
    
}

//implement FR checks
void write_uart(char character)
{
    //loop until not busy
    while(read_masked(regs->fr, 3, 3) != 0){}

    regs->data = write_masked(0, (unsigned int) character, 7, 0);
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
            if(character == '8'){
                spaces = true;
                character = *string++;
            }
            else if(character == '0'){
                if(*string++ != '8'){
                    //error
                }
                zeros = true;
                character = *string++;
            }

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
                kprintf(eight_number(unsigned_int_to_hex_str(value), spaces, zeros));
                character = *string++;
            }
            else if(character == 'i'){
                int value = va_arg(ap, int);
                kprintf(eight_number(int_to_dec_str(value), spaces, zeros));
                character = *string++;
            }
            else if(character == 'u'){
                unsigned int value = va_arg(ap, unsigned int);
                kprintf(eight_number(unsigned_int_to_dec_str(value), spaces, zeros));
                character = *string++;
            }
            else if(character == 'p'){
                void* value = va_arg(ap, void*);
                kprintf(eight_number(void_to_hex_str(value), spaces, false));
                character = *string++;
            }
            
            
        }
        write_uart(character);
        character = *string++;
    }
    va_end(ap);
}

//returns unsigned int of an masked unsigned int
//high -> msb position
//low -> lsb position
unsigned int read_masked(unsigned int value, int msb, int lsb)
{
    unsigned int mask = 0;
    for(int i=0; i<=msb-lsb; i++){
        mask |= 1 << i;
    } 
    value = value >> lsb;
    return value & mask;
}

unsigned int write_masked (unsigned int dest, unsigned int src, int msb, int lsb)
{
    unsigned int mask = 0;
    for(int i=0; i<=msb-lsb; i++){
        mask |= 1 << i;
    } 
    src = src << lsb;
    mask = mask << lsb;
    dest = dest & ~mask;
    dest = src | dest;
    return dest;
}

//returns a number string with 8 characters 
//if spaces == true it will be filled up with spaces
//if zeros == true it will be filled up with zeros
//only one is allowed to be set to true
char* eight_number(char* number, bool spaces, bool zeros)
{
    if(spaces == true && zeros == true){
        //error
    }
    int len = str_len(number);
    if(spaces == true){
        static char extendet_number[9] = "        ";
        for(int i=0; i<len; i++){
            extendet_number[8-len+i] = number[i];
        }
        return extendet_number;
    }
    if(zeros == true){
        static char extendet_number[9] = "00000000";
        if(*number == '-'){
            extendet_number[0] = '-';
            for(int i=1; i<len; i++){
                extendet_number[8-len+i] = number[i];
            }
        }
        else{
            for(int i=0; i<len; i++){
                extendet_number[8-len+i] = number[i];
            }
        }
        return extendet_number;
    }
    return number;
    
}

//length of string without '\0'
int str_len(char* string)
{
    int counter = 0;
    while(*string != '\0'){
        counter++;
        string++;
    }
    return counter;
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
    char conversion_list[16] = "0123456789abcdef";
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