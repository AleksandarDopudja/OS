//
// Created by os on 8/10/23.
//

extern "C" {
#include "../lib/console.h"
#include "../lib/hw.h"
}

#include "../h/syscall_cpp.hpp"

extern "C" void init_memory();
extern "C" void interrupt_routine();
extern "C" void* mem_alloc_kernel(size_t size);
extern "C" int mem_free_kernel(void *ptr);
extern "C" void* mem_alloc(size_t size);
extern "C" int mem_free(void* ptr);
extern "C" int thread_create(thread_t* handle,
                             void(*start_routine)(void*),
                             void* arg);
extern "C" int init_main_thread();
extern "C" void init_scheduler();
extern "C" void init_idle_thread();
extern "C" int thread_exit();
extern "C" void init_sem_console();

extern void userMain();

void intiAll(){
    init_memory();
    init_main_thread();
    init_sem_console();

    asm volatile ("csrw stvec, %0" : : "r"(interrupt_routine));
    init_scheduler();
    init_idle_thread();
}

void t1(void *a){

    for (int i = 0; i < 3; ++i) {
        //__putc(static_cast<char>('A' + *reinterpret_cast<uint64*>(a)));
        __putc('A' + *reinterpret_cast<uint64*>(a));
        __putc('\n');
        //if(*reinterpret_cast<uint64*>(a)==1) thread_exit();
        thread_dispatch();
    }
    __putc('k');
    __putc('r');
    __putc('a');
    __putc('j');
    __putc('A' + *reinterpret_cast<uint64*>(a));
    __putc('\n');
    thread_exit();
    //thread_dispatch();
}

int main() {

    intiAll();

    // u korisnickom rezimu se ovaj bit ignorise!
//    uint64 sstatus;
//    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
//    sstatus |= 1 << 1; // sttaus |= 0b10
//    asm volatile("csrw sstatus, %0" : : "r" (sstatus));

    userMain();

    //    char c = getc();
//    putc('A');
//    putc(c);
//    putc('A');
//    putc('\n');

//    thread_t t;
//    int a = 0;
//    thread_create( &t,t1, &a);
//
//    for(int i = 0; i<10; i++) {
//        __putc('m');
//        __putc('a');
//        __putc('i');
//        __putc('n');
//        __putc('\n');
//        thread_dispatch();
//    }
//    __putc('k');
//    __putc('r');
//    __putc('a');
//    __putc('j');
//    __putc('\n');

    return 0;
}
