/*
Concurrent access on a counter with no lock. Atomicity Violation. Data Race in line 15. Intra Region.
*/
#include <stdio.h>
#define N 100000

int countervar = 0;


int count(){
    #pragma omp target map(tofrom:countervar) device(0)
    #pragma omp teams num_teams(1)
    #pragma omp distribute parallel for
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