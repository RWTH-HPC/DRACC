/*
Matrix Vector multiplication with Matrix missing on Accelerator. Using the target enter data construct. The Matrix should be mapped and copied implicitly on the device.
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define C 5000


int *a;
int *b;
int *c;


int init(){
    for(int i=0; i<C; i++){
        for(int j=0; j<C; j++){
            b[j+i*C]=1;
        }
        a[i]=1;
        c[i]=0;
    }
        return 0;
}


int Mult(){
    
    #pragma acc data copyin(a[0:C]) copyout(c[0:C])
    {
        #pragma acc parallel loop gang
        for(int i=0; i<C; i++){
            #pragma acc loop worker
            for(int j=0; j<C; j++){
                c[i]+=b[j+i*C]*a[j];
            }
        }
    }        
    
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(c[i]!=C){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
    
    
}


int main(){
    a = malloc(C*sizeof(int));
    b = malloc(C*C*sizeof(int));
    c = malloc(C*sizeof(int));
    init();
    Mult();
    check();
    free(a);
    free(b);
    free(c);
    return 0;
}