/*
SAXPY without a barrier at the end of execution to wait for the tasks to finish.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define C 4096


float a;
float x[C];
float y[C];


int init(){
    for(int i=0; i<C; i++){
        a=5;
        x[i]=0;
        y[i]=3;
    }
        return 0;
}


int saxpy(){
    

    #pragma omp target map(to:y[0:C],a) map(tofrom:x[0:C]) nowait device(0)
    {
        for(int i=0; i<C; i++){
            #pragma omp task depend(inout:x[i])
            {
                x[i] = a * x[i];                
            }
            #pragma omp task depend(inout:x[i])
            {
                x[i] = x[i] + y[i];
            }
        }
    }        
    
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(x[i]!=3){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    init();
    saxpy();
    check();
	#pragma omp taskwait
    return 0;
}