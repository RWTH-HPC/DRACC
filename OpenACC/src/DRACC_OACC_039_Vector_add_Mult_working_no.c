/*
Vector addition then scalar multiplication without application errors. Inter and Intra Region.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 20
#define C 512


int a;
int b[C];
int c[C];

int init(){
    for(int i=0; i<C; i++){
        b[i]=0;
        c[i]=2;
    }
    a=2;
    return 0;
}


int add_Mult(){
#pragma acc data copyout(b[0:C]) copyin(c[0:C],a)
	for(int i=0;i<C;i++){
		b[i]=0;
	}
    for(int j=0; j<N ;j++){
        #pragma acc loop gang worker
        for(int i=0; i<C; i++){
            b[i] = (b[i] + c[i]) * a;
        }
    }   
    return 0;
}

int check(){
    bool test = false;
    int val = 0;
    for(int i=0; i<N; i++){
        val = (val + 2) * 2;
    }
    for(int i=0; i<C; i++){
        if(b[i]!=val){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
}

int main(){
    init();
    add_Mult();
    check();
    return 0;
}