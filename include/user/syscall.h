#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <user/exceptions.h>
#include <stdbool.h>


char syscall_getc();
void syscall_putc(char);
void syscall_sleep(unsigned int);
void syscall_exit()__attribute__((weak)); 
bool syscall_thread_create(void (*func)(void *), const void*, unsigned int);
bool syscall_process_create(void (*func)(void *), const void*, unsigned int);
void undef_syscall();



#endif