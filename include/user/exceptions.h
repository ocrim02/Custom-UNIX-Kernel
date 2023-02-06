#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

enum call_type{
    GETC = 0,
    PUTC = 1,
    EXIT = 2,
    SLEEP = 3,
    THREAD_CREATE = 4,
    PROCESS_CREATE = 5,
};

void create_prefetch_abort();
unsigned int create_supervisor_call(enum call_type, unsigned int, unsigned int, unsigned int)__attribute__((weak));
void create_undefined_instruction();
void create_data_abort();
void wait_for_int()__attribute__((weak));
void read_addr(unsigned int)__attribute__((weak));
void write_addr(unsigned int)__attribute__((weak));
void jump_addr(unsigned int)__attribute__((weak));
void stack_overflow();

#endif