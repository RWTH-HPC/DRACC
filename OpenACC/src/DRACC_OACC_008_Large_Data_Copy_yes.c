/*
Matrix Addition with large matrices, and copying them whole.
All Matrices are to big to fit on the accelerator as a whole, resulting in a segmentation fault.
*/
#include <stdio.h>   
#include <stdlib.h> 
#include <stdbool.h>
#include <stdint.h>


uint64_t C = 51200;

uint64_t *a;
uint64_t *b;
uint64_t *c;


int init(){
    for(uint64_t i=0; i<C; i++){
        for(uint64_t j=0; j<C; j++){
            a[j+i*C]=1;
            b[j+i*C]=1;
            c[j+i*C]=0;
        }
        
    }
        return 0;
}


int add(){
    #pragma acc data copyin(a[0:C*C],b[0:C*C]) copyout(c[0:C*C])
    {
        #pragma acc parallel loop gang
        for(uint64_t i=0; i<C; i++){
            #pragma acc loop worker vector
            for(uint64_t j=0; j<C; j++){
                c[j+i*C]=b[j+i*C] + a[j+i*C];
            }
        }
    }
    return 0;
}

int check(){
    bool test = false;
    for(uint64_t i=0; i<C*C; i++){
        if(c[i]!=2){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    a = malloc(C*C*sizeof(uint64_t));
    b = malloc(C*C*sizeof(uint64_t));
    c = malloc(C*C*sizeof(uint64_t));
    init();
    add();
    check();
    free(a);
    free(b);
    free(c);
    return 0;
}