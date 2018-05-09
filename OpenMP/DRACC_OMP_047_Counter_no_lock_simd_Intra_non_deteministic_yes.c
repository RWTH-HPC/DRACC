/*
Concurrent access on a counter with no lock with simd. Atomicity Violation. Data Race in line 28. Intra Region.
Forcing the execution to use more threads per team than the accelerator can handle per Warp, resulting in implicit Inter Region Data Races.

*/
#include <stdio.h>
#include <stdbool.h>
#define N 100000
#define C 64

int countervar[C];


int init(){
    for(int i=0; i<C; i++){
        countervar[i]=0;
    }
    return 0;
}



int count(){
    #pragma omp target map(tofrom:countervar[0:C]) device(0)
    #pragma omp teams num_teams(1) thread_limit(1048)
    #pragma omp distribute parallel for
        for (int i=0; i<N; i++){
            #pragma omp simd
            for(int i=0; i<C; i++){
                countervar[i]++;
            }
        }
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(countervar[i]!=N){
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