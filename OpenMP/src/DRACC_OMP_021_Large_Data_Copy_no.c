/*
Matrix Addition with large matrices, and copying them whole.
All Matrices are to big to fit on the accelerator whole resulting in a segmentation fault. Executes in host fallback.
*/
#include <stdio.h>   
#include <stdlib.h> 
#include <stdbool.h>
#include <stdint.h>
#define C 51200L


int64_t *a;
int64_t *b;
int64_t *c;


int init(){
    for(int64_t i=0; i<C; i++){
        for(int64_t j=0; j<C; j++){
            a[j+i*C]=1;
            b[j+i*C]=1;
            c[j+i*C]=0;
        }
        
    }
        return 0;
}


int add(){
    #pragma omp target map(to:a[0:C*C],b[0:C*C]) map(from:c[0:C*C]) device(0)
    {
        #pragma omp teams
        printf("Executed on host: %s\n",omp_is_initial_device() ? "true" : "false");
        #pragma omp teams distribute parallel for collapse(2)
        for(int64_t i=0; i<C; i++){
            for(int64_t j=0; j<C; j++){
                c[j+i*C]=b[j+i*C] + a[j+i*C];
            }
        }
    }
    return 0;
}

int check(){
    bool test = false;
    for(int64_t i=0; i<C*C; i++){
        if(c[i]!=2){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    a = (int64_t *) malloc(C*C*sizeof(int64_t));
    b = (int64_t *) malloc(C*C*sizeof(int64_t));
    c = (int64_t *) malloc(C*C*sizeof(int64_t));
    init();
    add();
    check();
    free(a);
    free(b);
    free(c);
    return 0;
}