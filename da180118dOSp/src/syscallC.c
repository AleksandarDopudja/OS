//
// Created by os on 8/10/23.
//
#include "../h/syscall_c.h"
//#include "../lib/console.h"

extern void* mem_alloc_abi(uint64, size_t);
extern int mem_free_abi(uint64, void*);
extern int thread_create_abi(uint64 code,
                             thread_t* handle,
                             void(*start_routine)(void*),
                             void* arg,
                             void* stack_space);
extern int thread_exit_abi(uint64 code);
extern void thread_dispatch_abi(uint64);

extern void interrupt_routine();

extern int sem_open_abi(uint64,sem_t*,unsigned );
extern int sem_close_abi(uint64,sem_t);
extern int sem_signal_abi(uint64,sem_t);
extern int sem_wait_abi(uint64,sem_t);
extern int getID_abi(uint64);


void* mem_alloc(size_t size) {

    size_t blks = size / MEM_BLOCK_SIZE;
    if(size % MEM_BLOCK_SIZE > 0) blks+=1;

    return mem_alloc_abi(0x01, blks);
}

int mem_free(void* ptr) {

    return mem_free_abi(0x02, ptr);
}

int thread_create(thread_t* handle,
                  void(*start_routine)(void*),
                  void* arg) {

    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if(stack == 0) return -1;
    return thread_create_abi(0x11, handle, start_routine, arg, stack);
}

int thread_exit() {

    int ret = thread_exit_abi(0x12);
    if(ret == 0) {
        thread_dispatch();
    }
    return ret;
}

void thread_dispatch(){

    thread_dispatch_abi(0x13);
}


int sem_open(sem_t* handle, unsigned init){

    return sem_open_abi(0x21,handle,init);
}

int sem_close(sem_t handle){

    return sem_close_abi(0x22,handle);
}

int sem_signal (sem_t i){

    return sem_signal_abi(0x24,i);
    //samo to dov
}

int sem_wait(sem_t i){

    int ret = sem_wait_abi(0x23,i);
    if(ret == -3){
        thread_dispatch();
        ret = 0;
    }
    return ret;
}


extern void putc_abi(uint64,char);
extern char getc_abi(uint64);
extern void predji_abi(uint64);

//sem_t sem_getc;
char getc(){
    //init
    predji_abi(0x56);
    return __getc();
}

void putc(char c){
    //putc_abi(0x42,c);
    __putc(c);
}

uint64 getID(){
    return getID_abi(0x50);
}