/*
Matrix Vector multiplication with another thread on the host constantly updating the values of a.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <omp.h>
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
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp target map(to:a[0:C],b[0:C*C]) map(from:c[0:C]) device(0)
            {
                #pragma omp teams distribute parallel for
                for(int i=0; i<C; i++){
                    for(int j=0; j<C; j++){
                        c[i]+=b[j+i*C]*a[j];
                    }
                }
            }
        #pragma omp section
        #pragma omp parallel num_threads(4)
            #pragma omp for
            for(int i=C; i>0; i--){
                a[i]=3;    
                #pragma omp target update to(a[i]) device(0)
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
    printf("Data Race occured: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    a = malloc(C*sizeof(int));
    b = malloc(C*C*sizeof(int));
    c = malloc(C*sizeof(int));
    init();
    omp_set_nested(1);
    Mult();
    check();
    free(a);
    free(b);
    free(c);
    return 0;
}