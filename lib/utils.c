#include <lib/utils.h>

#define INT_SIZE 32
#define UNSIGNED_INT_SIZE 32
#define VOID_SIZE 32
#define MAX_INT_STRING_SIZE 11
#define MAX_UNSIGNED_INT_STRING_SIZE 10
#define MAX_NUMBER_SIZE 12
#define NULL ((void *)0)

/*
 * own version of std printf()
 * almost same functionallity
 */
void kprintf(char* string, ...)
{
    
    va_list ap;
    va_start(ap, string);
    bool spaces = false;
    bool zeros = false;
    
    char character = *string++;
    char number_string[MAX_NUMBER_SIZE]; 
    char buffer_string[MAX_NUMBER_SIZE]; 
    while(character != '\0'){
        if(character != '%'){
            write_uart(character);
            character = *string++;
        }
        else{
            character = *string++;
            if(character == '8'){
                spaces = true;
                character = *string++;
            }
            else if(character == '0'){
                if(*string++ != '8'){
                    kprintf("\nERROR expected %%08!\n");
                    return;
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
                char* number = eight_number(number_string, unsigned_int_to_hex_str(buffer_string, value), spaces, zeros);
                if(number == NULL){return;}
                kprintf(number);
                character = *string++;
            }
            else if(character == 'i'){
                int value = va_arg(ap, int);
                char* number = eight_number(number_string, int_to_dec_str(buffer_string, value), spaces, zeros);
                if(number == NULL){return;}
                kprintf(number);
                character = *string++;
            }
            else if(character == 'u'){
                unsigned int value = va_arg(ap, unsigned int);
                char* number = eight_number(number_string, unsigned_int_to_dec_str(buffer_string, value), spaces, zeros);
                if(number == NULL){return;}
                kprintf(number);
                character = *string++;
            }
            else if(character == 'p'){
                void* value = va_arg(ap, void*);
                char* number = eight_number(number_string, void_to_hex_str(buffer_string, value), spaces, false);
                if(number == NULL){return;}
                kprintf(number);
                character = *string++;
            }
            else if(character == '%'){
                write_uart('%');
                character = *string++;
            }
            else{
                kprintf("\nERROR single %%. Use %%%% if you want to print a single %%.\n");
                return;
            }

        }
        
    }
    va_end(ap);
    return;
}

/* 
 * takes unsigned int value and two bit positions msb >= lsb
 * returns msb to lsb from value (shifted to first bit)
 */
unsigned int read_masked(unsigned int value, int msb, int lsb)
{
    unsigned int mask = 0;
    for(int i=0; i<=msb-lsb; i++){
        mask |= 1 << i;
    } 
    value = value >> lsb;
    return value & mask;
}

/* 
 * reverse function of read_masked 
 * writes src to dest(msb downto lsb) without changing other bits
 */
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

/*
 * returns a number string with 8 characters if one bool is set true, else return number
 * if spaces == true it will be filled up with spaces
 * if zeros == true it will be filled up with zeros
 * only one is allowed to be set to true
 */
char* eight_number(char* extendet_number, char* number, bool spaces, bool zeros)
{
    if(spaces == true && zeros == true){
        kprintf("\nERROR while calling eight_number(char*, bool, bool), only one bool can be set True.\n");
    }

    int len = str_len(number);

    //fill with spaces
    if(spaces == true && len <= 8){
        //init char array
        for(int i=0; i<8; i++){
            extendet_number[i] = ' ';
        }
        extendet_number[8] = '\0';

        for(int i=0; i<len; i++){
            extendet_number[8-len+i] = number[i];
        }
        return extendet_number;
    }
    //fill with zeros
    else if(zeros == true && len <= 8){
        //init char array
        for(int i=0; i<8; i++){
            extendet_number[i] = '0';
        }
        extendet_number[8] = '\0';

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
    //error number to big
    else if((zeros == true || spaces == true) && len > 8){
        kprintf("\nERROR while calling eight_number(char*, bool, bool), char* is longer than 8 characters.\n");
        return NULL;
    }
    else{
        return number;
    }
    
}

/*
 * length of string without '\0'
 */
int str_len(char* string)
{
    int counter = 0;
    while(*string != '\0'){
        counter++;
        string++;
    }
    return counter;
}

/* 
 * converts unsigned int to char array
 */
char* unsigned_int_to_dec_str (char* string, unsigned int value)
{
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

/*
 * converts int to char array
 */
char* int_to_dec_str (char* string, int value)
{
    int negatives_offset = 0;
    if(value < 0){
        string[0] = '-';
        negatives_offset = 1;
        value = value * (-1);
    }
    char output[MAX_NUMBER_SIZE];
    char* rest = unsigned_int_to_dec_str(output, (unsigned int) value);
    for(int i=0; i<= MAX_UNSIGNED_INT_STRING_SIZE; i++){
        string[i + negatives_offset] = rest[i];
    }

    return string;
    

}

/*
 * converts an unsigned int to hex char array
 */
char* unsigned_int_to_hex_str(char* string, unsigned int value)
{
    //catch value
    if(value == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return string;
    }

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

/*
 * converts a void pointer to a hex char array
 */
char* void_to_hex_str(char* string, void* pointer)
{
    string[0] = '0';
    string[1] = 'x';
    char buffer[MAX_NUMBER_SIZE];
    char* content = unsigned_int_to_hex_str(buffer, (unsigned int) pointer);
    for(int i=0; i<(UNSIGNED_INT_SIZE/4); i++){
        string[i+2] = content[i];
    }
    return string;
}