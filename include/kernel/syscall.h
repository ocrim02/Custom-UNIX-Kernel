#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <arch/cpu/exception_creator.h>
#include <stdbool.h>


char syscall_getc() __attribute__((weak));
void syscall_putc(char) __attribute__((weak));
void syscall_sleep(unsigned int) __attribute__((weak));
void syscall_exit() __attribute__((weak)); 
bool syscall_thread_create(void (*func)(void *), const void*, unsigned int) __attribute__((weak));
void undef_syscall() __attribute__((weak));



#endif