/*
Matrix Vector multiplication with the Matrix partially missing on Accelerator. Using the target enter data construct.
The missing matrix will be copied implicitly as well as the result vector "c", but c will be copied out a second time resulting in a cuda error.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define C 512


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
    #pragma acc enter data copyin(a[0:C]) create(c[0:C])
    {
        #pragma acc parallel loop gang
        for(int i=0; i<C; i++){
            #pragma acc loop worker
            for(int j=0; j<C; j++){
                c[i]+=b[j+i*C]*a[j];
            }
        }
    }
    #pragma acc exit data copyout(c[0:C]) delete(a[0:C])
    return 0;
}

int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(c[i]!=C){
            test = true;
        }
    }
    printf("Data Race occured: %s\n",test ? "true" : "false");
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