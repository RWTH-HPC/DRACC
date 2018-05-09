/*
Concurrent access on a counter with no lock. Atomicity Violation. Data Race in line 14. Intra and Inter Region.
*/
#include <stdio.h>
#define N 100000

int countervar = 0;


int count(){
    #pragma acc parallel copy(countervar) num_workers(256)
    #pragma acc loop gang worker reduction(+:countervar)
        for (int i=0; i<N; i++){
            countervar++;
        }
    return 0;
}


int main(){
    count();
    printf("counter: %i expected: 100000\n ",countervar);
    return 0;
}