//
// Created by os on 8/10/23.
//
#include "../h/syscall_cpp.hpp"

//extern "C" void* mem_alloc_kernel(size_t size);

void* operator new(size_t size){

    //return mem_alloc_kernel(size/MEM_BLOCK_SIZE + 1);
    return mem_alloc(size);
}

void operator delete(void * ptr){
    mem_free(ptr);
}

Thread::~Thread() {
    //thread_exit();
    //thread_dispatch();
}
