/*
Matrix Vector multiplication without allocating the matrix on the device.
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Grid dimension
#define B 100

//Block dimension
#define T 256

//Array size
#define C B*T

// Macro for checking errors in CUDA API calls
#define cudaErrorCheck(call)                                                              \
do{                                                                                       \
    cudaError_t cuErr = call;                                                             \
    if(cudaSuccess != cuErr){                                                             \
      printf("CUDA Error - %s:%d: '%s'\n", __FILE__, __LINE__, cudaGetErrorString(cuErr));\
      exit(0);                                                                            \
    }                                                                                     \
}while(0)

//Host pointer for matrix b, input vector a and result vector c
int *a;
int *b;
int *c;

//Device pointer for matrix d_b, input vector d_a and result vector d_c
int *d_a;
int *d_b;
int *d_c;

//Initialization and allocation of the host variables
int init(){
    //Allocating host variables
    a = (int *) malloc(C*sizeof(int));
    b = (int *) malloc(C*C*sizeof(int));
    c = (int *) malloc(C*sizeof(int));
    
    //Initialize host values
    for(int i=0; i<C; i++){
        for(int j=0; j<C; j++){
            b[j+i*C]=1;
        }
        a[i]=1;
        c[i]=0;
    }
        return 0;
}

//Kernel
__global__ void Mult(int* d_a, int* d_b, int* d_c){
    int tid = blockDim.x * blockIdx.x + threadIdx.x;
    
    for(int j=0; j<C; j++){
        d_c[tid]+=d_b[j+tid*C]*d_a[j];
    }
}

//Checking if the values stored in c are correct
int check(){
    bool test = false;
    for(int i=0; i<C; i++){
        if(c[i]!=C){
            test = true;
        }
    }
    printf("Data Race occured: %s\n",test ? "true\n" : "false\n");
    return 0;
    
    
}

//Initialization of the variables on the GPU 
int initcuda(){
    //Allocation of GPU memory for d_a,d_c
    cudaErrorCheck( cudaMalloc(&d_a, C*sizeof(int)));
    //cudaErrorCheck( cudaMalloc(&d_b, C*C*sizeof(int)));
    cudaErrorCheck( cudaMalloc(&d_c, C*sizeof(int)));
    
    //Copying the array a and the matrix b from the host to the array d_a and the matrix d_b on the device
    cudaErrorCheck( cudaMemcpy(d_a,a,C*sizeof(int),cudaMemcpyHostToDevice));
    cudaErrorCheck( cudaMemcpy(d_b,b,C*C*sizeof(int),cudaMemcpyHostToDevice));
    
    return 0;
}

//Main programm
int main(){
    //Calling the initialization methods
    init();
    initcuda();
    
    //Launch Kernel
    Mult<<<B,T>>>(d_a,d_b,d_c);
    
    // Check for errors in kernel launch (e.g. invalid execution configuration paramters)
    cudaErrorCheck( cudaGetLastError());

    // Check for errors on the GPU after control is returned to CPU
    cudaErrorCheck( cudaDeviceSynchronize());
    
    //Copying back the result d_c from the device to the host array c
    cudaErrorCheck( cudaMemcpy(c,d_c,C*sizeof(int),cudaMemcpyDeviceToHost)); 
    
    //Verify result
    check();
    
    //Freeing GPU memory
    cudaErrorCheck( cudaFree(d_a));
    cudaErrorCheck( cudaFree(d_b));
    cudaErrorCheck( cudaFree(d_c));
    
    //Freeing CPU memory
    free(a);
    free(b);
    free(c);
    return 0;
}