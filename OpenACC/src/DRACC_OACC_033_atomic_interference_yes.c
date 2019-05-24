/*
Incrementaion with a threshold of N. The incrementation is atomic, but the query on the threshold depends on the counter.
The query is not atomic, therefore a Data Race is possible in line 15 to 17.
*/
#include <stdio.h>
#define N 100000

int countervar = 0;


int count(){
    #pragma acc parallel copy(countervar) num_workers(256)
    #pragma acc loop gang worker
        for (int i=0; i<N*2; i++){
            if(countervar<N){
                #pragma acc atomic update
                countervar++;
            }
        }
    return 0;
}


int main(){
    count();
    printf("counter: %i expected: %i\n ",countervar,N);
    return 0;
}