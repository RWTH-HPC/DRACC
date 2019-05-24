/*
Concurrent access on a counter with the wrong lock, by utilising OpenMP Lock Routines. Atomicity Violation.
Two locks are used to ensure that addition and substraction cannot be interrupted by themselfes on other teams. 
Although they are able to interrupt eachother leading to a wrong result. Inter Region.
*/
#include <omp.h>
#include <stdio.h>

#define N 100

int countervar = 0;
#pragma omp declare target
omp_lock_t addlock;
omp_lock_t sublock;
#pragma omp end declare target


int count(){
    #pragma omp target map(tofrom:countervar) device(0)
    #pragma omp teams 
    {
    omp_init_lock(&addlock);
    omp_init_lock(&sublock);
    #pragma omp distribute
    for(int i=0; i<N; i++){
        
        omp_set_lock(&addlock);
        countervar++;
        omp_unset_lock(&addlock);
        omp_set_lock(&sublock);
        countervar -= 2;
        omp_unset_lock(&sublock);
        
    }
    omp_destroy_lock(&addlock);
    omp_destroy_lock(&sublock);
    }
    return 0;
}

int main(){
    
    
    
    count();
    printf("counter: %i expected: -%i\n ",countervar,N);
    
    return 0;
}