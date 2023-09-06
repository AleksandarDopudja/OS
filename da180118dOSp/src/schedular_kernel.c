//
// Created by os on 8/10/23.
//
#include "../h/syscall_c.h"

#define MAX_THREADS 256

//extern void* mem_alloc_kernel(size_t size);
extern void* mem_alloc(size_t size);

typedef struct thread_sch{
    thread_t thread;
    struct thread_sch* next;
} thread_sch;

thread_sch *list, *first_free , *first, *last;
//last_free?

void put(thread_t newt){

    thread_sch *oldf = first_free;
    //first_free->thread = newt;
    oldf->thread = newt;

    if(first_free->next == 0){

        thread_sch *newf = (thread_sch*)((uint64)first_free+sizeof(thread_sch));

        if((uint64)newf >= ((uint64)list+MAX_THREADS*sizeof(thread_sch))) return; //greska

        newf->thread = 0;
        newf->next = 0;

        first_free->next = 0;
        first_free = newf;
    }else{
        first_free = first_free->next;
    }

    if(first == last && first == 0 && last== 0){
        oldf->next = 0; //dodao 12.8.23.
        first = last = oldf;
    }else{
        oldf->next = 0; //dodao
        last->next = oldf;
        last = oldf;
    }

}

thread_t get(){

    if(first == 0) return 0;

    thread_t ret = first->thread;
    thread_sch* old = first;

    first = first->next;
    if(first == 0) last = 0;

    //free old
    old->thread = 0;
    if(first_free == 0){
        old->next = 0;
        first_free = old;
    }
    else if((uint64)old < (uint64)first_free){
        old->next = first_free;
        first_free = old;
    }else{
        thread_sch *curr = first_free;
        thread_sch *prev = 0;

        while(curr && ((uint64)curr < (uint64)old)){
            prev = curr;
            curr = curr->next;
        }

        if(curr){
            prev->next = old;
            old->next = curr;
        }else{
            prev->next = old;
        }

    }


    return  ret;
}

void init_scheduler(){

    size_t size = MAX_THREADS * sizeof(thread_sch);
    list = (thread_sch*) mem_alloc(size);
    //list = (thread_sch*) mem_alloc_kernel(size/MEM_BLOCK_SIZE+1);

    first_free = (thread_sch*)((uint64)list);
    first_free->thread = 0;
    first_free->next = 0;

    first = 0;
    last = 0;
}
