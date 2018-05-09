/*
Concurrent access on a counter with no lock. Atomicity Violation. Data Race in line 15. Intra Region.
Forcing the execution to use more threads per team than the accelerator can handle per Warp, resulting in implicit Inter Region Data Races.
*/
#include <stdio.h>
#define N 100000

int countervar = 0;


int count(){
    #pragma omp target map(tofrom:countervar) device(0)
    #pragma omp teams num_teams(1) thread_limit(1048)
    #pragma omp distribute parallel for
        for (int i=0; i<N; i++){
            countervar++;
        }
    return 0;
}

int main(){
    count();
    printf("counter: %i expected: %i\n",countervar,N);
    return 0;
}