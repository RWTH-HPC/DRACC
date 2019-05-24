/*
SAXPY without dependencies between the addition and multiplication task.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define C 20000


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
    
    #pragma omp target map(to:y[0:C],a) map(tofrom:x[0:C]) device(0)
    {
        #pragma omp teams distribute        
        for(int i=0; i<2*C; i++){
            if(i%2==0){
                #pragma omp task 
                {
                    x[i] = a * x[i];                
                }
            }else
            {
                #pragma omp task 
                {
                    x[i] = x[i] + y[i];
                }
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
    return 0;
}