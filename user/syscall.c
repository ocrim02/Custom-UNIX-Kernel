#include <user/syscall.h>


char syscall_getc(){
    return (char) create_supervisor_call(GETC, 0, 0, 0);
}

void syscall_putc(char c){
    create_supervisor_call(PUTC, (unsigned int) c, 0, 0);
}

void syscall_exit(){
    create_supervisor_call(EXIT, 0, 0, 0);
}

void syscall_sleep(unsigned int duration){
    if(duration == 0){
        return;
    }
    create_supervisor_call(SLEEP, duration, 0, 0);
}

bool syscall_thread_create(void (*func)(void *), const void* args, unsigned int arg_size){
    return (bool) create_supervisor_call(THREAD_CREATE, (unsigned int) func, (unsigned int) args, arg_size);
}

bool syscall_process_create(void (*func)(void *), const void* args, unsigned int arg_size){
    return (bool) create_supervisor_call(PROCESS_CREATE, (unsigned int) func, (unsigned int) args, arg_size);
}

void undef_syscall(){
    create_supervisor_call(5, 0, 0, 0);
}