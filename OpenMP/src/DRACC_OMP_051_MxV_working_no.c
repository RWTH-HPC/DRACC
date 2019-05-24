/*
Matrix Vector multiplication without application errors.
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
	#pragma omp target map(to:a[0:C],b[0:C*C]) map(from:c[0:C])
    #pragma omp teams distribute parallel for
    for(int i=0; i<C; i++){
        for(int j=0; j<C; j++){
            c[i]+=b[j+i*C]*a[j];
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