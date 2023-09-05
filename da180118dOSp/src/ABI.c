//
// Created by os on 8/10/23.
//

#include "../lib/hw.h"

struct _thread;
typedef struct _thread* thread_t;
struct _sem;
typedef struct _sem* sem_t;

void* mem_alloc_abi(uint64 code, size_t size){ //
    void *ret;
    asm volatile("ecall");//sistemski poziv
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
};

int mem_free_abi(uint64 code, void* ptr){
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
};

int thread_create_abi(uint64 code,
                      thread_t* handle,
                      void(*start_routine)(void*),
                      void* arg,
                      void* stack_space) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}

int thread_exit_abi(uint64 kod) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}

void thread_dispatch_abi(uint64 kod) {
    asm volatile("ecall");
}

int sem_open_abi(uint64 kod,sem_t * handle,unsigned init) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}

int sem_close_abi(uint64 kod,sem_t id) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}


int sem_wait_abi(uint64 kod,sem_t id) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}


int sem_signal_abi(uint64 kod,sem_t id) {
    int ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}

void putc_abi(uint64 kod,char c){
    asm volatile("ecall");
}

char getc_abi(uint64 kod){
    char ret;
    asm volatile("ecall");
    asm volatile("mv %0 , a0" : "=r"(ret));
    return ret;
}