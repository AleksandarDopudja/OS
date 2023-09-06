//
// Created by os on 8/21/23.
//
#include "../lib/console.h"

void console_handler1(){
    console_handler();
}

void ilegalInst(){
    char a = 'A';
    __putc(a);
}

char getc_kernel(){
    char a = __getc();
    return a;
}

void putc_kernel(char c){
    __putc(c);
}


