#include <kernel/thread.h>

#define PCR_USR_IFT 16
#define IDLE_THREAD_COUNT 1
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
    enum WAITING_FOR waiting_reason;
    unsigned int timer_intervals;
};

struct tcb tcbs[THREAD_COUNT + IDLE_THREAD_COUNT];

struct tcb *runqueue;

struct tcb *running_thread = NULL;


/*
* replace currently running thread with the next (reasonable) thread
*/
void change_thread(struct dump_regs* regs, enum THREAD_STATE reason){
    struct tcb* next_thread = next_in_queue();

    if(next_thread->id == THREAD_COUNT && running_thread == next_thread){
        return;
        //continue in idle
    }
    if(reason != Finished && reason != Waiting && running_thread->id != THREAD_COUNT && next_thread->id == THREAD_COUNT){
        return;
        //no change due to no new task and old task is not finsihed
    }
    if(next_thread->state != Ready){
        kprintf("Error: selected Thread not ready!\n");
        return;
    }
    
    if(running_thread != NULL){
        for(int i=0; i<13; i++){
            running_thread->r[i] = regs->r[i];
        }
        running_thread->lr = regs->lr;
        running_thread->sp = regs->sp;
        running_thread->cpsr = regs->spsr;
        running_thread->pc = regs->pc;
        running_thread->state = reason;
        if(reason == Finished || running_thread->id == THREAD_COUNT){
            reset_thread(running_thread);
        }
        else if(reason == Waiting){
            add_to_queue_end(running_thread, true);
        }
        else{
            add_to_queue_end(running_thread, false);
        }
    }

    for(int i=0; i<13; i++){
        regs->r[i] = next_thread->r[i];
    }
    regs->lr = next_thread->lr;
    regs->sp = next_thread->sp;
    regs->spsr = next_thread->cpsr;
    regs->pc = next_thread->pc;
    next_thread->state = Running;

    running_thread = next_thread;

}


/*
* returns a thread that waits for an interrupt
*/
struct tcb* idle_thread(){
    tcbs[THREAD_COUNT].pc = (unsigned int) &wait_for_int;
    tcbs[THREAD_COUNT].lr = (unsigned int) &syscall_exit;
    tcbs[THREAD_COUNT].cpsr = PCR_USR_IFT;
    tcbs[THREAD_COUNT].state = Ready;

    return &tcbs[THREAD_COUNT];
}


bool thread_create(void (*func)(void *), const void *args, unsigned int args_size){
    struct tcb *free_thread = get_free_thread();
    if(free_thread == NULL){
        //No free thread available
        return false;
    }

    for(unsigned int i=args_size; i>0; i--){
        *((char*) free_thread->sp) = *((char*) args);
        free_thread->sp = free_thread->sp - 1;
        args = args + 1;
    }
    free_thread->sp = free_thread->sp + args_size;

    free_thread->pc = (unsigned int) func;
    free_thread->lr = (unsigned int) &syscall_exit;
    free_thread->cpsr = PCR_USR_IFT;
    free_thread->r[0] = free_thread->sp;
    free_thread->r[1] = args_size;
    free_thread->state = Ready;

    add_to_queue_start(free_thread);
    return true;
}


struct tcb* get_free_thread(){
    if(runqueue->rq_prev->rq_prev->state != Finished){
        return NULL;
    }
    return runqueue->rq_prev->rq_prev;
}


void init_threads(){
    for(int i=0; i<(THREAD_COUNT); i++){
        tcbs[i].rq_next = &tcbs[(i+1)%THREAD_COUNT];
        tcbs[i].rq_prev = &tcbs[(i+(THREAD_COUNT-1))%THREAD_COUNT];
        tcbs[i].state = Finished;
        tcbs[i].id = i;
        tcbs[i].sp = (THREAD_SP_BASE - THREAD_SP_SIZE * i);
        tcbs[i].timer_intervals = 0;
        tcbs[i].waiting_reason = 0;
    }
    runqueue = &tcbs[0];

    tcbs[THREAD_COUNT].rq_next = NULL;
    tcbs[THREAD_COUNT].rq_prev = NULL;
    tcbs[THREAD_COUNT].state = Ready;
    tcbs[THREAD_COUNT].id = THREAD_COUNT;
    tcbs[THREAD_COUNT].sp = (THREAD_SP_BASE - THREAD_SP_SIZE * THREAD_COUNT);
}


/*
* makes given thread the next in the queue
*/
void add_to_queue_start(struct tcb* thread){
    if(thread == runqueue){
        kprintf("Error 1\n");
        return;
    }

    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;

    runqueue->rq_prev->rq_next = thread;
    thread->rq_prev = runqueue->rq_prev;
    thread->rq_next = runqueue;
    runqueue->rq_prev = thread;

    runqueue = thread;
}


/*
* adds given thread to the end of the queue elements that are not Finished yet
*/
void add_to_queue_end(struct tcb* thread, bool waiting_end){
    struct tcb* queue_end = thread->rq_prev;
    if(waiting_end){
        while((queue_end->state == Finished || queue_end->state == Ready) && queue_end != runqueue){
            queue_end = queue_end->rq_prev;
        }
    }
    else{
        while(queue_end->state == Finished && queue_end != runqueue){
            queue_end = queue_end->rq_prev;
        }
    }
    
    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;

    if(queue_end == runqueue){
        runqueue->rq_prev->rq_next = thread;
        thread->rq_prev = runqueue->rq_prev;
        thread->rq_next = runqueue;
        runqueue->rq_prev = thread;

        runqueue = thread;
    }
    else{
        thread->rq_next = queue_end->rq_next;
        thread->rq_prev = queue_end;
    
        queue_end->rq_next->rq_prev = thread;
        queue_end->rq_next = thread;
    }

}


struct tcb* next_in_queue(){
    struct tcb* start = runqueue;
    while(start->state == Waiting){
        start = start->rq_next;
    }
    if(start->state != Ready){
        return idle_thread();
    }

    if(start != runqueue){
        add_to_queue_start(start);
    }
    runqueue = start->rq_next;
    return start;
}


void reset_thread(struct tcb* thread){
    thread->state = Finished;
    thread->sp = (THREAD_SP_BASE + THREAD_SP_SIZE * thread->id);
}


/*
* the first thread in the queue waiting for uart input is set to Ready state
*/
void uart_wake(){
    struct tcb* pointer = runqueue;
    int counter = 0;
    while(pointer->state == Waiting && pointer->waiting_reason != Uart && counter < THREAD_COUNT){
        pointer = pointer->rq_next;
        counter++;
    }
    if(pointer->state == Waiting && pointer->waiting_reason == Uart){
        pointer->state = Ready;
    }
    
}

/*
* time left to wait for all timer waiting threads is reduced by 1 interval
* if interval is 0 afterwars thread state is set to Ready
*/
void timer_update(){
    struct tcb* pointer = runqueue;
    int counter = 0;
    while(pointer->state == Waiting && counter < THREAD_COUNT){
        if(pointer->waiting_reason == Timer){
            pointer->timer_intervals = pointer->timer_intervals - 1;
            if(pointer->timer_intervals == 0){
                pointer->state = Ready;
            }
        }
        pointer = pointer->rq_next;
        counter++;
    }
}

void set_uart_sleep(){
    runqueue->rq_prev->waiting_reason = Uart;
}

void set_timer_sleep(unsigned int intervals){
    runqueue->rq_prev->waiting_reason = Timer;
    runqueue->rq_prev->timer_intervals = intervals;
}



