/*
Concurrent access on a counter with no lock. Atomicity Violation. Data Race in line 14. Only using worker level parallelism (Intra Region).
*/
#include <stdio.h>
#define N 100000

int countervar = 0;


int count(){
    #pragma acc parallel num_workers(256) copy(countervar)
    #pragma acc loop worker
        for (int i=0; i<N; i++){
            countervar++;
        }
    return 0;
}

int main(){
    count();
    printf("counter: %i expected: %i\n ",countervar,N);
    return 0;
}