/*
Concurrent access on a counter with the wrong lock, by utilising OpenMP Lock Routines and simd. Atomicity Violation.
Two locks are used to ensure that addition and substraction cannot be interrupted by themselfes on other teams. 
Although they are able to interrupt eachother leading to a wrong result. Inter Region.
*/
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#define N 100
#define C 512

#pragma omp declare target
omp_lock_t addlock;
omp_lock_t sublock;
#pragma omp end declare target
int countervar[C];


int init(){
    for(int i=0; i<C; i++){
        countervar[i]=0;
    }
    return 0;
}

int count(){
    #pragma omp target map(tofrom:countervar[0:C]) device(0)
    #pragma omp teams 
    {
    omp_init_lock(&addlock);
    omp_init_lock(&sublock);
    #pragma omp distribute
    for(int j=0; j<N; j++){
        
        omp_set_lock(&addlock);
        #pragma omp simd
        for(int i=0; i<C; i++){
            countervar[i]++;
        }
        omp_unset_lock(&addlock);
        omp_set_lock(&sublock);
        #pragma omp simd
        for(int i=0; i<C; i++){
            countervar[i]-=2;
        }
        omp_unset_lock(&sublock);
        
    }
    omp_destroy_lock(&addlock);
    omp_destroy_lock(&sublock);
    }
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(countervar[i]!=-N){
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