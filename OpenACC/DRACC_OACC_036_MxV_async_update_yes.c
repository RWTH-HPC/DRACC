/*
Matrix Vector multiplication with an update of a variable during asynchroneous execution of the kernel.
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
	#pragma acc enter data create(a[0:C])
        return 0;
}

int Update(){
	for(int i=C-1;i>=0;i--){
		a[i] = C;
		#pragma acc data copyin(a[i])
		{}
	}
	return 0;
}


int Mult(){
    #pragma acc parallel loop gang copyin(a[0:C],b[0:C*C]) copyout(c[0:C]) async
    for(int i=0; i<C; i++){
        #pragma acc loop worker
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
    printf("Data Race occured: %s\n",test ? "true" : "false");
	#pragma acc exit data delete(a[0:C])
    return 0;
}

int main(){
    a = malloc(C*sizeof(int));
    b = malloc(C*C*sizeof(int));
    c = malloc(C*sizeof(int));
    init();
    Mult();
	Update();
	#pragma acc wait
    check();    
    free(a);
    free(b);
    free(c);
    return 0;
}