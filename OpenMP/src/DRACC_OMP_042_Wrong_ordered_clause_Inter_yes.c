/*
Data race between the values in countervar leading to changing results, by utilising the ordered construct the execution will be sequentially consistent.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 42000

int countervar[N];

int init(){
    for(int i=0; i<N; i++){
        countervar[i]=0;
    }
    return 0;
}

int count(){
    #pragma omp target map(tofrom:countervar[0:N]) device(0)
    #pragma omp teams distribute
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