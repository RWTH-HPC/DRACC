/*
Vector addition then scalar multiplication with no implicit barrier in between.
*/


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 20
#define C 512


int a;
int b[C];
int c[C];
int temp[C];

int init(){
    for(int i=0; i<C; i++){
        b[i]=0;
        c[i]=2;
        temp[i]=0;
    }
    a=2;
    return 0;
}


int add_Mult(){
#pragma acc data copy(b[0:C]) copyin(c[0:C],temp[0:C],a)
    {

        for(int j =0; j<N; j++){
        #pragma acc loop gang vector        
            for(int i=0; i<C; i++){
                temp[i] = b[i] + c[i];
            }    
        
        #pragma acc loop gang vector        
            for(int i=C; i>0; i--){
                b[i] = temp[i] * a;
            }    
            
        
        }
    }    
    return 0;
}

int check(){
    bool test = false;
    int val = 0;
    for(int i=0; i<N; i++){
        val = val + 2;
        val = val * 2;
    }
    for(int i=0; i<C; i++){
        if(b[i]!=val){
            test = true;
        }
    }
    printf("Data Race occured: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    init();
    add_Mult();
    check();
    return 0;
}