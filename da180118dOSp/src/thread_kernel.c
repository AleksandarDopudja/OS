//
// Created by os on 8/10/23.
//

#include "../lib/hw.h"
#include "../lib/console.h"

#define Sys_Stack_Size 1024

extern  void* mem_alloc_kernel(size_t size);
extern int mem_free_kernel(void* ptr);
extern int thread_exit();

struct _sem;
typedef struct _sem* sem_t;

struct _thread{

    uint64 pc;
    uint64 sp;
    uint64 ssp;

    uint64 state; //0-init, 1-term, 2-running, 3-ready, 4-blocked, 5-end

    void* stack;
    void* sysstack;
    void(*start_routine)(void*);
    void* arg;

    sem_t sem;
};

typedef struct _thread* thread_t;

extern void put(thread_t);
extern thread_t get();
extern void thread_dispatch();
extern int thread_create(thread_t* handle,
                         void(*start_routine)(void*),
                         void* arg);

thread_t running; //thread-PCB
thread_t idle_thread;
thread_t main_thread;

extern int sem_close(sem_t);

void wrapper(){
    running->start_routine(running->arg);
    sem_close(running->sem);
    thread_exit();
    //thread_dispatch();
}

extern int sem_open_kernel(sem_t * handle,unsigned );

int thread_create_kernel(thread_t* handle,
                         void(*start_routine)(void*),
                         void* arg,
                         void* stack_space){

    uint64 size = sizeof(struct _thread) / MEM_BLOCK_SIZE;
    if((sizeof(struct _thread) % MEM_BLOCK_SIZE) > 0) size+=1;

    *handle = mem_alloc_kernel(size);

    if(*handle == 0){
        return -1;
    }

    size  = Sys_Stack_Size / MEM_BLOCK_SIZE;
    if((Sys_Stack_Size % MEM_BLOCK_SIZE) > 0) size+=1;
    void* sstack_space = mem_alloc_kernel(size);

    if(stack_space == 0){
        mem_free_kernel(stack_space);
        mem_free_kernel(*handle);
        return -1;
    }

    thread_t newt = *handle;
    sem_open_kernel(&(newt->sem),0);

    newt->state = 3;
    newt->stack = stack_space;
    newt->sysstack = sstack_space;
    //newt->sp = ((uint64)stack_space + DEFAULT_STACK_SIZE - 1) & ~0xF; // poslednja 4 bita na 0 -> vrednost poravnata na 16B
    newt->sp = ((uint64)stack_space + DEFAULT_STACK_SIZE - ((uint64)stack_space%16));
    newt->sp -= 32;
    //newt->ssp = ((uint64)sstack_space + Sys_Stack_Size - 1) & ~0xF;
    newt->ssp = ((uint64)sstack_space + Sys_Stack_Size - ((uint64)sstack_space%16));
    newt->ssp -= 256;
    //newt->pc = (uint64)wrapper;
    newt->start_routine = start_routine;
    newt->arg = arg;

    for(int i=0; i<32; i++) ((uint64*)newt->ssp)[i] = 0;

    ((uint64*)newt->ssp)[1] = newt->pc;
    newt->pc = (uint64)wrapper;

    //ubaci u sheduler
    put(newt);

    return 0;
}

int thread_exit_kernel(){

    running->state = 5; //end
    int free_stack = mem_free_kernel(running->stack);
    int free_sstack = mem_free_kernel(running->sysstack);
    int free_struct = mem_free_kernel(running);

    if(free_stack != 0 || free_sstack != 0 || free_struct != 0) return -1;

    return 0;
}

void thread_dispatch_kernel(){

    if(running->state != 5 && running->state != 4) put(running);

    thread_t newt = get();
    if(newt == 0) newt = idle_thread; //running==idle_thread
    if (newt != running){
        asm volatile("csrr %0, sepc":"=r"(running->pc));
        asm volatile("csrw sepc, %0"::"r"(newt->pc));
        running = newt;
    }
}

void thread_block(){
    running->state = 4;
}

void thread_unblock(thread_t thr){
    thr->state = 3;
    //put(t)? i kada bi to dodao onda bi morao i kod block-a da izbacujem taj thread iz schedulera
}

void idle_thread_function(void *args){

    //uint64 sstatus_val;
    //asm volatile("csrr %[sstatus], sstatus" : [sstatus] "=r" (sstatus_val));

    //uint64 spp_bit = (sstatus_val >> 8) & 1;

    while(1){
        __putc('I');
        __putc('d');
        __putc('l');
        __putc('e');
        __putc('\n');
        thread_dispatch();
    }

}

void init_idle_thread(){

    thread_create(&idle_thread,idle_thread_function,0);
    idle_thread->state = 4; //block

}

int init_main_thread(){

    uint64 size = sizeof(struct _thread) / MEM_BLOCK_SIZE;
    if((sizeof(struct _thread) % MEM_BLOCK_SIZE) > 0) size+=1;

    main_thread = mem_alloc_kernel(size);

    if(main_thread == 0) return -1;

    size  = Sys_Stack_Size / MEM_BLOCK_SIZE;
    if((Sys_Stack_Size % MEM_BLOCK_SIZE) > 0) size+=1;
    void* sstack_space = mem_alloc_kernel(size);

    if(sstack_space == 0){
        mem_free_kernel(main_thread);
        return  -1;
    }

    main_thread->sysstack = sstack_space;
    //main_thread->ssp = ((uint64)sstack_space + Sys_Stack_Size - 1) & ~0xF;
    main_thread->ssp = ((uint64)sstack_space + Sys_Stack_Size - ((uint64)sstack_space%16));
    main_thread->ssp -= 256;
    main_thread->state = 2;

    running = main_thread;

    return 0;
}

//sem_t get_sem_thread(thread_t t){
//    return t->sem;
//}