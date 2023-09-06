//
// Created by os on 8/10/23.
//

extern "C" {
#include "../lib/console.h"
#include "../lib/hw.h"
}

#include "../h/syscall_cpp.hpp"


extern "C" void interrupt_routine();
extern "C" void* mem_alloc_kernel(size_t size);
extern "C" int mem_free_kernel(void *ptr);
//extern "C" void* mem_alloc(size_t size);
//extern "C" int mem_free(void* ptr);
//extern "C" int thread_create(thread_t* handle,
//                             void(*start_routine)(void*),
//                             void* arg);
extern "C" void init_memory();

extern "C" void init_scheduler();
extern "C" int init_main_thread();
extern "C" void init_idle_thread();
extern "C" int thread_exit();


extern void userMain();

void intiAll(){

    init_memory();
    init_main_thread();
    //init

    asm volatile ("csrw stvec, %0" : : "r"(interrupt_routine));

    init_scheduler();
    init_idle_thread();
}

int finished = 0;
void t1(void *a){

    for (int i = 0; i < 10; ++i) {
        //__putc(static_cast<char>('A' + *reinterpret_cast<uint64*>(a)));
        __putc('A' + ((uint64)getID()-2));
        __putc('0' + (getID()-1));
        //__putc('\n');
        //if(*reinterpret_cast<uint64*>(a)==1) thread_exit();
        thread_dispatch();
    }

    finished++;

//    __putc('k');
//    __putc('r');
//    __putc('a');
//    __putc('j');
//    __putc('A' + *reinterpret_cast<uint64*>(a));
//    __putc('\n');

    //thread_exit();

    //thread_dispatch();
}

int main() {

    intiAll();

    // u korisnickom rezimu se ovaj bit ignorise!
//    uint64 sstatus;
//    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
//    sstatus &= 0x0000000000000100; // sttaus |= 0b10
//    asm volatile("csrw sstatus, %0" : : "r" (sstatus));

    //asm volatile("csrr t6, sepc");

    //thread_dispatch();

    userMain();

    //putc('0'+(uint64)getID());
    //putc('\n');
    //char c = getc();
    //putc('A');
    //putc('B');
    //putc(c);
    //putc('A');
    //putc('\n');

//     thread_t t, t2, t3;
//     thread_create( &t,t1, 0);
//    thread_create( &t2,t1, 0);
//    thread_create( &t3,t1, 0);

//    while (finished!=3)thread_dispatch();
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
