#ifndef _THREAD_H_
#define _THREAD_H_

#include <arch/cpu/cpu.h>
#include <arch/cpu/exception_handler.h>
#include <arch/cpu/exception_creator.h>

enum THREAD_STATE{
    Ready = 0,
    Running = 1,
    Waiting = 2,
    Finished = 3,
};

typedef struct dump_regs dump_regs;


void change_thread(struct dump_regs*, enum THREAD_STATE);
void thread_create(void (*func)(void *), const void*, unsigned int);
struct tcb* get_free_thread();
void init_threads();
void add_to_queue_start(struct tcb*);
void add_to_queue_end(struct tcb*);
void reset_thread(struct tcb*);
struct tcb* idle_thread();
struct tcb* next_in_queue();


#endif /* THREAD */