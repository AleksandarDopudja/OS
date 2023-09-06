//
// Created by os on 8/10/23.
//
//#include "../lib/console.h"

void timer(){
    unsigned long sipreg;
    asm volatile("csrr %0, sip" : "=r" (sipreg));
    sipreg &= ~2; //1. bit(SSIE) - maskiranje soft prekida
    asm volatile("csrw sip, %0" : : "r" (sipreg));
    //console_handler();
}