#ifndef _EXCEPTION_CREATOR_H_
#define _EXCEPTION_CREATOR_H_

enum call_type{
    GETC = 0,
    PUTC = 1,
    EXIT = 2,
    SLEEP = 3,
    THREAD_CREATE = 4,
};

void create_prefetch_abort();
unsigned int create_supervisor_call(enum call_type, unsigned int, unsigned int, unsigned int)__attribute__((weak));
void create_undefined_instruction();
void create_data_abort();
void wait_for_int();

#endif