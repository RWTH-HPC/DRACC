/*
Concurrent access on a counter with the wrong lock, by utilising OpenMP critical directives and simd. Atomicity Violation.
Two locks are used to ensure that addition and substraction cannot be interrupted by them selfes on other teams. 
Although they are able to interrupt eachother leading to a wrong result. Intra Region.
Because of the lockstep, the team size and the mapping of threads on the accelerator this code will not be able to cause a Data Race.
If the Data Race is induced depends in the implementation of the critical construct on accelerators.
*/
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>

#define N 10000
#define C 512

int countervar[C];


int init(){
    for(int i=0; i<C; i++){
        countervar[i]=0;
    }
    return 0;
}



int count(){
    #pragma omp target map(tofrom:countervar[0:C]) device(0)
    #pragma omp teams num_teams(1)
    #pragma omp distribute parallel for
    for(int j=0; j<N; j++){
        
        #pragma omp critical(addlock)
        #pragma omp simd
        for(int i=0; i<C; i++){
            countervar[i]++;
        }
        
        #pragma omp critical(sublock)
        #pragma omp simd
        for(int i=0; i<C; i++){
            countervar[i] -= 2;
        }
        
        
    }
    
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(countervar[i]!= -N){
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