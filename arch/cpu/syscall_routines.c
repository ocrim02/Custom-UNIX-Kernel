#include <arch/cpu/syscall_routines.h>

void getc_routine(struct dump_regs * regs){
    char c = pop_ring_buffer();
    if(c == 0){
        set_uart_sleep();
        change_thread(regs, Waiting);            
    }
    else{
        regs->r[0] = (unsigned int) c;
        regs->pc = regs->pc + 4;
    }
}

void putc_routine(struct dump_regs * regs){
    write_uart(regs->r[1]);
    regs->pc = regs->pc + 4;
}

void exit_routine(struct dump_regs * regs){
    change_thread(regs, Finished);
}

void sleep_routine(struct dump_regs * regs){
    set_timer_sleep(regs->r[1]);
    regs->pc = regs->pc + 4;
    change_thread(regs, Waiting);
}

void thread_create_routine(struct dump_regs * regs){
    regs->r[0] = thread_create((void*) regs->r[1], (void*) regs->r[2], regs->r[3], false); 
    regs->pc = regs->pc + 4;
}

void process_create_routine(struct dump_regs * regs){
    regs->r[0] = thread_create((void*) regs->r[1], (void*) regs->r[2], regs->r[3], true); 
    regs->pc = regs->pc + 4;
}