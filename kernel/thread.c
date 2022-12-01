#include <arch/cpu/cpu.h>
#include <arch/cpu/exception_handler.h>
#include <arch/cpu/exception_creator.h>

#define PCR_USR_IFT 240
#define THREAD_COUNT 32
#define NULL 0

//aus VL7
struct tcb{
    unsigned int r[13];
    unsigned int sp;
    unsigned int lr;
    unsigned int pc;
    unsigned int cpsr;
    unsigned int state;   
    unsigned int id;
    struct tcb *rq_next;
    struct tcb *rq_prev;
};

enum{
    Ready = 0,
    Running = 1,
    Waiting = 2,
    Finished = 3,
};

/*
runqueue points to thread selected to run
runqueue->next is the next in the list
| runqueue
            <- prev
first     second     third
next ->             
*/

struct tcb tcbs[THREAD_COUNT];

struct tcb *runqueue;

void change_thread(struct dump_regs* regs, unsigned int reason){
    struct tcb* current_thread = runqueue;
    struct tcb* next_thread = runqueue->rq_next;
    runqueue = next_thread;

    for(int i=0; i<13; i++){
        current_thread->r[i] = regs->r[i];
    }
    current_thread->lr = regs->lr;
    current_thread->sp = regs->sp;
    current_thread->cpsr = regs->spsr;
    current_thread->pc = regs->pc;
    current_thread->state = reason;
    if(reason == Finished){
        reset_thread(current_thread);
    }
    else{
        add_to_queue_end(current_thread);
    }

    for(int i=0; i<13; i++){
        regs->r[i] = next_thread->r[i];
    }
    regs->lr = next_thread->lr;
    regs->sp = next_thread->sp;
    regs->spsr = next_thread->cpsr;
    regs->pc = next_thread->pc;
    next_thread->state = Running;


}

void thread_create(void (*func)(void *), const void *args, unsigned int args_size){
    struct tcb *free_thread = get_free_thread();
    if(free_thread == NULL){
        kprintf("No free thread available\n");
        return;
    }

    free_thread->pc = func;
    free_thread->lr = (unsigned int) &create_supervisor_call;
    free_thread->cpsr = PCR_USR_IFT;
    free_thread->r[0] = args;
    free_thread->r[1] = args_size;

    free_thread->state = Ready;

    //wo in der schlange positionieren?
    add_to_queue_start(free_thread);

}

struct tcb* get_free_thread(){
    if(runqueue->rq_prev->state != Finished){
        kprintf("No free Thread");
        return NULL;
    }
    return runqueue->rq_prev;
}

void init_threads(){
    for(int i=0; i<THREAD_COUNT; i++){
        tcbs[i].rq_next = &tcbs[(i+1)%THREAD_COUNT];
        tcbs[i].rq_prev = &tcbs[(i-1)%THREAD_COUNT];
        tcbs[i].state = Finished;
        tcbs[i].id = i;
        tcbs[i].sp = (THREAD_SP_BASE + THREAD_SP_SIZE * i);
    }
    runqueue = &tcbs[0];
}

void add_to_queue_start(struct tcb* thread){
    if(thread == runqueue){
        kprintf("Error\n");
        return;
    }

    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;

    thread->rq_prev = runqueue;
    thread->rq_next = runqueue->rq_next;
    runqueue->rq_next->rq_prev = thread;
    runqueue->rq_next = thread;
}

void add_to_queue_end(struct tcb* thread){
    struct tcb* queue_end = runqueue->rq_prev;
    while(queue_end->state == Finished && queue_end != runqueue){
        queue_end = queue_end->rq_prev;
    }

    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;
    
    queue_end->rq_next->rq_prev = thread;
    queue_end->rq_next = thread;
}


void reset_thread(struct tcb* thread){
    thread->state = Finished;
    thread->sp = (THREAD_SP_BASE + THREAD_SP_SIZE * thread->id);
}
/*
* stackpointer zurück auf ursprung
*/
