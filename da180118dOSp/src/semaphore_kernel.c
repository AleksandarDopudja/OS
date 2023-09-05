//
// Created by os on 8/13/23.
//

#include "../lib/hw.h"

#define MAX_THREADS_BLOCKED 64

struct _thread;
typedef struct _thread* thread_t;

extern void * mem_alloc_kernel(size_t size);
extern int mem_free_kernel(void * ptr);
extern void put(thread_t);
extern void thread_block();
extern void thread_unblock(thread_t t);
extern thread_t running;

typedef struct thread_node{
    thread_t thread;
    struct  thread_node* next;
} thread_node;

struct _sem{
    unsigned i;
    thread_node *thread_list;
    thread_node *first;
    thread_node *last;
};

typedef struct _sem* sem_t;

thread_node* get_free_node(sem_t sem) {

    thread_node* ret = sem->thread_list;
    int num = 0;

    while (num<MAX_THREADS_BLOCKED){
        ret = (thread_node*)((uint64)ret+num*sizeof(struct thread_node));
        if(ret->thread==0) return ret;
        num++;
    }

    return 0;
}

int sem_open_kernel (
        sem_t* handle,
        unsigned init
) {

    uint64 size = sizeof(struct _sem) / MEM_BLOCK_SIZE;
    if((sizeof(struct _sem) % MEM_BLOCK_SIZE) > 0) size+=1;

    *handle = mem_alloc_kernel(size);

    if(*handle == 0) return -1;

    (*handle)->i = init;

    size = (sizeof(struct thread_node)*MAX_THREADS_BLOCKED) / MEM_BLOCK_SIZE;
    if(((sizeof(struct thread_node)*MAX_THREADS_BLOCKED) % MEM_BLOCK_SIZE) > 0) size+=1;
    (*handle)->thread_list = (thread_node*)mem_alloc_kernel(size);

    (*handle)->first = 0;
    (*handle)->last = 0;

    thread_node* ret =  (*handle)->thread_list;
    int num = 0;

    while (num<MAX_THREADS_BLOCKED){

        ret = (thread_node*)((uint64)ret+num*sizeof(struct thread_node));
        ret->thread = 0;
        ret->next = 0;
        num++;
    }

    return 0;
}

int sem_close_kernel(sem_t sem){

    thread_node* tmp = sem->first;

    while (tmp){

        thread_unblock(tmp->thread);
        put(tmp->thread);

        tmp->thread = 0;
        thread_node* next = tmp->next;
        tmp->next = 0;

        tmp = next;
    }

    //sem->first = sem->last = 0;

    mem_free_kernel(sem->thread_list);
    mem_free_kernel(sem);

    return 0;
}

int sem_wait_kernel(sem_t sem){

    if(sem->i != 0){
        sem->i -= 1;
    }else{
        thread_node* newtn = get_free_node(sem);
        if(newtn==0) return -1;
        newtn->thread = running;
        thread_block();
        if(sem->first == 0) sem->first = newtn;
        if(sem->last != 0) sem->last->next = newtn;
        sem->last = newtn;

        return -3;
    }

    return 0;
}

int sem_signal_kernel(sem_t sem){

    if(sem->first == 0){
        sem->i += 1;
    }else{

        thread_unblock(sem->first->thread);
        put(sem->first->thread);

        sem->first->thread = 0;
        thread_node* next = sem->first->next;
        sem->first->next = 0;

        sem->first = next;
        if(next == 0) sem->last = 0;
    }

    return 0;
}

extern sem_t sem_getc;

void sem_getc_signal(){
    sem_signal_kernel(sem_getc);
}

void init_sem_console(){
    sem_open_kernel(&sem_getc,0);
}