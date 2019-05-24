/*
Data race between the values in countervar leading to changing results. Only using worker level parallelism.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 100000

int countervar[N];

int init(){
    for(int i=0; i<N; i++){
        countervar[i]=0;
    }
    return 0;
}

int count(){
    #pragma acc data copy(countervar[0:N])
    #pragma acc parallel num_workers(256)
    #pragma acc loop worker
        for (int i=1; i<N; i++){
            countervar[i]=countervar[i-1]+1;
        }
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<N; i++){
        if(countervar[i]!=i){
            test = true;
        }
    }
    
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    
    return 0;
}

int main(){
    init();
    count();
    check();
    return 0;
}