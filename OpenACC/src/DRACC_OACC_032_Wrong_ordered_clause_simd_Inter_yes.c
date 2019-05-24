/*
Data race between the values in countervar leading to changing results with simd. Not using workerlevel parallelism.
.*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 42000
#define C 16

int countervar[N];

int init(){
    for(int i=0; i<N; i++){
        countervar[i]=0;
    }
    return 0;
}

int count(){
    #pragma acc data copy(countervar[0:N])
    #pragma acc parallel vector_length(8)
    #pragma acc loop gang vector
        for (int i=C; i<N; i++){
            countervar[i]=countervar[i-C]+1;
        }
    return 0;
}

int check(){
    bool test = false;
    for(int i=C; i<N; i++){
        if(countervar[i]!=i-C+1){
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