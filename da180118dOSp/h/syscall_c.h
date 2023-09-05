//
// Created by os on 8/10/23.
//

#ifndef PROBA_SYSCALL_C_H
#define PROBA_SYSCALL_C_H

#include "../lib/hw.h"
#include "../lib/console.h"

void* mem_alloc(size_t size);

int mem_free(void* ptr);

struct  _thread;
typedef struct _thread* thread_t;

int thread_create(thread_t* handle,
                  void(*start_routine)(void*),
                  void* arg);

int thread_exit();

void thread_dispatch();

struct _sem;
typedef struct _sem* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

char getc();

void putc(char);

#endif //PROBA_SYSCALL_C_H
