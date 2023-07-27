/*
Concurrent access on a counter with the wrong lock, by utilising OpenMP critical directives. Atomicity Violation.
Two locks are used to ensure that addition and subtraction cannot be interrupted by themselves on other teams. 
Although they are able to interrupt each other leading to a wrong result. Intra Region. 
Because of the lockstep, the team size and the mapping of threads on the accelerator this code will not be able to cause a Data Race.
If the Data Race is induced depends in the implementation of the critical construct on accelerators.
*/
#include <omp.h>
#include <stdio.h>

#define N 100000

int countervar = 0;



int count(){
    #pragma omp target map(tofrom:countervar) device(0)
    #pragma omp teams num_teams(1)
    #pragma omp distribute parallel for
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