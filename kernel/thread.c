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
};

/*
runqueue points to thread selected to run NEXT
| runqueue
            <- prev
first     second     third
next ->             
*/

struct tcb tcbs[THREAD_COUNT + IDLE_THREAD_COUNT];

struct tcb *runqueue;

struct tcb *running_thread = NULL;

void change_thread(struct dump_regs* regs, enum THREAD_STATE reason){
    struct tcb* next_thread = next_in_queue();

    if(next_thread->id == THREAD_COUNT && running_thread == next_thread){
        return;
        //no change because no new task and old task is not finsihed
    }
    if(reason != Finished && running_thread->id != THREAD_COUNT && next_thread->id == THREAD_COUNT){
        return;
    }

    kprintf("\n");
    //kprintf("next id %u, ", next_thread->id);
    //kprintf("next state %u, ", next_thread->state);
    //kprintf("reason: %u, ", reason);
    //kprintf("running: %u   ", running_thread->id);

    //testing only
    if(next_thread->state != Ready){
        kprintf("Error: selected Thread not ready!\n");
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
        else{
            add_to_queue_end(running_thread);
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

struct tcb* idle_thread(){
    tcbs[32].pc = (unsigned int) &wait_for_int;
    tcbs[32].lr = (unsigned int) &create_supervisor_call;
    tcbs[32].cpsr = PCR_USR_IFT;
    tcbs[32].state = Ready;
    //tcbs[32].rq_next = runqueue->rq_next;
    //tcbs[32].rq_prev = runqueue;

    return &tcbs[32];
}

void thread_create(void (*func)(void *), const void *args, unsigned int args_size){
    //kprintf("Create Thread\n");
    struct tcb *free_thread = get_free_thread();
    if(free_thread == NULL){
        kprintf("No free thread available\n");
        return;
    }

    for(unsigned int i=args_size; i>0; i--){
        *((char*) free_thread->sp) = *((char*) args);
        free_thread->sp = free_thread->sp - 1;
        args = args + 1;
    }
    free_thread->sp = free_thread->sp + args_size;

    free_thread->pc = (unsigned int) func;
    free_thread->lr = (unsigned int) &create_supervisor_call;
    //kprintf("%x\n", (unsigned int) &create_supervisor_call);
    //kprintf("%p\n", &create_supervisor_call);
    free_thread->cpsr = PCR_USR_IFT;
    free_thread->r[0] = free_thread->sp;
    free_thread->r[1] = args_size;

    /*kprintf("Thread: \n");
    kprintf("%x\n", free_thread->pc);
    kprintf("%x\n", free_thread->lr);
    kprintf("%x\n", free_thread->cpsr);
    kprintf("%x\n", free_thread->r[0]);
    kprintf("%x\n", free_thread->r[1]);*/

    free_thread->state = Ready;

    //wo in der schlange positionieren?
    add_to_queue_start(free_thread);

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
    }
    runqueue = &tcbs[0];
    //running_thread = &tcbs[THREAD_COUNT];

    tcbs[THREAD_COUNT].rq_next = NULL;
    tcbs[THREAD_COUNT].rq_prev = NULL;
    tcbs[THREAD_COUNT].state = Ready;
    tcbs[THREAD_COUNT].id = THREAD_COUNT;
    tcbs[THREAD_COUNT].sp = (THREAD_SP_BASE - THREAD_SP_SIZE * THREAD_COUNT);
}

void add_to_queue_start(struct tcb* thread){
    if(thread == runqueue){
        kprintf("Error\n");
        return;
    }

    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;

    runqueue->rq_prev->rq_next = thread;
    thread->rq_prev = runqueue->rq_prev;
    thread->rq_next = runqueue;
    runqueue->rq_prev = thread;

    runqueue = thread;

    /*kprintf("add to queue start\n");
    struct tcb* pointer = runqueue;
    for(int i=0; i<32; i++){
        kprintf("ID: %u\n", pointer->id);
        kprintf("state: %u\n", pointer->state);
        pointer = pointer->rq_next;
    }*/
}

void add_to_queue_end(struct tcb* thread){
    struct tcb* queue_end = thread->rq_prev;
    while(queue_end->state == Finished && queue_end != runqueue){
        queue_end = queue_end->rq_prev;
    }
    //kprintf("queue end: %u\n", queue_end->id);
    //kprintf("runqueue: %u\n", runqueue->id);
    //kprintf("thread prev: %u\n", thread->rq_prev->id);

    thread->rq_next->rq_prev = thread->rq_prev;
    thread->rq_prev->rq_next = thread->rq_next;

    if(queue_end == runqueue){
        //kprintf("same\n");
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

    /*struct tcb* pointer = runqueue;
    for(int i=0; i<32; i++){
        kprintf("ID: %u\n", pointer->id);
        kprintf("state: %u\n", pointer->state);
        pointer = pointer->rq_next;
    }*/
    
    
}

struct tcb* next_in_queue(){
    if(runqueue->state != Ready){
        return idle_thread();
    }
    runqueue = runqueue->rq_next;
    return runqueue->rq_prev;
}


void reset_thread(struct tcb* thread){
    //kprintf("reset Thread id: %u\n", thread->id);
    thread->state = Finished;
    thread->sp = (THREAD_SP_BASE + THREAD_SP_SIZE * thread->id);
}

/*struct tcb* current_thread(){
    return runqueue;
}*/

