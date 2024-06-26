#ifndef _THREAD_H_
#define _THREAD_H_

#include <arch/cpu/cpu.h>
#include <arch/cpu/exception_handler.h>
#include <user/syscall.h>
#include <user/exceptions.h>

#define PCR_USR_IFT 16
#define IDLE_THREAD_COUNT 1
#define THREAD_COUNT 32
#define NULL 0
#define PROCESS_COUNT 9

enum THREAD_STATE{
    Ready = 0,
    Running = 1,
    Waiting = 2,
    Finished = 3,
};

enum WAITING_FOR{
    Uart = 0,
    Timer = 1,
};

typedef struct dump_regs dump_regs;


void change_thread(struct dump_regs*, enum THREAD_STATE);
bool thread_create(void (*func)(void *), const void*, unsigned int, bool new_pid);
struct tcb* get_free_thread();
void init_threads();
void add_to_queue_start(struct tcb*);
void add_to_queue_end(struct tcb*, bool);
void reset_thread(struct tcb*);
struct tcb* idle_thread();
struct tcb* next_in_queue();
void uart_wake();
void timer_update();
void set_uart_sleep();
void set_timer_sleep(unsigned int);


#endif /* THREAD */