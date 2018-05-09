/*
Concurrent access on a counter with the wrong lock, by utilising OpenMP critical directives. Atomicity Violation.
Two locks are used to ensure that addition and substraction cannot be interrupted by themselfes on other teams. 
Although they are able to interrupt each other leading to a wrong result. Inter and Intra Region.
*/
#include <omp.h>
#include <stdio.h>

#define N 100000

int countervar = 0;



int count(){
    #pragma omp target map(tofrom:countervar) device(0)
    #pragma omp teams distribute parallel for
    for(int i=0; i<N; i++){
        
        #pragma omp critical(addlock)
        countervar++;
        
        
        #pragma omp critical(sublock)
        countervar -= 2;
        
        
    }
    
    return 0;
}

int main(){
    

    count();
    printf("counter: %i expected: -%i\n",countervar,N);
    
    return countervar;
}