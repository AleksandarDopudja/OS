//
// Created by os on 8/10/23.
//
#include "../lib/hw.h"

#define HEADER_SIZE sizeof(BlockHeader)

typedef struct BlockHeader{
    int size; //u blokovima
    struct BlockHeader* next;
} BlockHeader;

static BlockHeader* heapStart = 0;
static  BlockHeader* freeMemStart = 0;
//static uint64 limit = 0;

void* mem_alloc_kernel(uint64 numOfBlks){

    BlockHeader* curr = freeMemStart;
    BlockHeader* prev = 0;

    while(curr && curr->size<numOfBlks){
        prev = curr;
        curr = curr->next;
    }

    //if(!curr || ((uint64)curr>=limit)) return 0;
    if(!curr) return 0;

    int size = curr->size;
    int dif = size - numOfBlks;
    curr->size = numOfBlks;

    //ako je slobodni blok veci od trazenog prostora
    if(dif != 0) {
        BlockHeader *newBH = (BlockHeader *) ((uint64)curr + numOfBlks * (MEM_BLOCK_SIZE + HEADER_SIZE));
        newBH->size = dif;

        if(prev == 0) {
            //newBH->next = 0;
            newBH->next = curr->next;
            freeMemStart = newBH;
        }
        else{
            newBH->next = curr->next;
            prev->next = newBH;
        }
    }else{
        if(prev == 0) {
            freeMemStart = 0;
        }
        else{
            prev->next = curr->next;
        }
    }

    return (void*)((uint64)curr+HEADER_SIZE);

}

int mem_free_kernel(void* ptr){

    //van opsega
    if(((uint64)ptr < (uint64)HEAP_START_ADDR) || ((uint64)ptr > (uint64)HEAP_END_ADDR)) return -1;

    //adr nije poravnata
    //if(((uint64)ptr-HEADER_SIZE)%(MEM_BLOCK_SIZE+HEADER_SIZE)!=0) return -1;

    BlockHeader* curr2 = heapStart;

    uint64 headerAddr = (uint64)ptr - HEADER_SIZE;
    BlockHeader* blockToFree = (BlockHeader*)headerAddr;

    while(curr2 && curr2!=blockToFree && curr2<blockToFree){
        curr2 = (BlockHeader*)((uint64)curr2 + curr2->size*(MEM_BLOCK_SIZE+HEADER_SIZE));
    }

    BlockHeader* curr = freeMemStart;
    BlockHeader* prev = 0;

    while (curr && curr < blockToFree){
        prev = curr;
        curr = curr->next;
    }

    //vec je free taj deo mem
    if(curr == blockToFree) return -1;

    //da li je adresa dobijena preko mem_alloc
    if(curr2 != blockToFree) return -1;

    if(curr == 0){

        if(prev == 0){
            freeMemStart = blockToFree;
            blockToFree->next = 0;
        }
        else{
            if((uint64)prev+prev->size*(MEM_BLOCK_SIZE+HEADER_SIZE) == (uint64)blockToFree){
                prev->size += blockToFree->size;
            }
        }
    }
    else{

        if(prev == 0){

            if((uint64)curr == (uint64)blockToFree + blockToFree->size*(MEM_BLOCK_SIZE+HEADER_SIZE)){
                blockToFree->size += curr->size;
                blockToFree->next = curr->next;
            }
            else{
                blockToFree->next = curr;
            }

            freeMemStart = blockToFree;
        }
        else{
            int in = 0;
            if((uint64)curr == (uint64)blockToFree+blockToFree->size*(MEM_BLOCK_SIZE+HEADER_SIZE)){
                blockToFree->size += curr->size;
                blockToFree->next = curr->next;
                prev->next = blockToFree;
                in=1;
            }

            if((uint64)prev+prev->size*(MEM_BLOCK_SIZE+HEADER_SIZE) == (uint64)blockToFree){
                prev->size += blockToFree->size;
                if(in) {
                    prev->next = blockToFree->next;
                }
            }
        }
    }

    return 0;
}

void init_memory(){

    uint64 numOfBlks = ((uint64)(HEAP_END_ADDR) - (uint64)HEAP_START_ADDR) / MEM_BLOCK_SIZE;

    heapStart = (BlockHeader*)HEAP_START_ADDR;
    heapStart->size = numOfBlks;
    heapStart->next = 0;

    freeMemStart = heapStart;
    //limit = (uint64)HEAP_START_ADDR + numOfBlks* (MEM_BLOCK_SIZE + HEADER_SIZE);
}