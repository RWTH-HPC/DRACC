/*
Data race between the values in countervar leading to changing results with simd. by utilising the ordered construct the execution will be sequentially consistent.
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
    #pragma omp target map(tofrom:countervar[0:N]) device(0)
    #pragma omp teams distribute simd safelen(C)
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
    printf("Data Race occured: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    init();
    count();
    check();
    return 0;
}