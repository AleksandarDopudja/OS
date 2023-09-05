//
// Created by os on 8/10/23.
//

#ifndef PROBA_SYSCALL_CPP_HPP
#define PROBA_SYSCALL_CPP_HPP

extern "C"{
#include "syscall_c.h"
}

#define MAX_THREADS 128

void* operator new(size_t size);

void operator delete(void* ptr);

class Thread {
public:

    Thread (void (*body)(void*), void* arg){
        this->arg=arg;
        this->body=body;
    };

    virtual ~Thread ();

    int start (){
        return thread_create(&myHandle,body,arg);
    };

    static void dispatch (){
        thread_dispatch();
    };

protected:

    Thread (){
        body = wrapper;
        arg = this;
    };

    virtual void run () {}

private:

    thread_t myHandle;
    void (*body)(void*);
    void* arg;

    static void wrapper(void* arg){
        Thread* t = (Thread*)arg;
        t->run();
    }
};

class Semaphore {
public:

    Semaphore (unsigned init = 1){
        sem_open(&(this->myHandle),init);
    }

    ~Semaphore (){
        sem_close(myHandle);
    }

    int wait (){
        return sem_wait(myHandle);
    }

    int signal (){
        return sem_signal(myHandle);
    }

    void* operator new(size_t size){

        return mem_alloc(size);
    }

    void operator delete(void* ptr){
        mem_free(ptr);
    }

private:
    sem_t myHandle;
};

class Console {
public:
    static char getc (){
        return ::getc();
    }
    static void putc (char a){
        ::putc(a);
    }
};

#endif //PROBA_SYSCALL_CPP_HPP
