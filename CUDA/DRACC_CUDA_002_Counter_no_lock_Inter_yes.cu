/*
Concurrent access on a counter with no lock. Atomicity Violation. Data Race in line 26. Inter Region Data Race.
*/
#include <stdio.h>

// Macro for checking errors in CUDA API calls
#define cudaErrorCheck(call)                                                              \
do{                                                                                       \
    cudaError_t cuErr = call;                                                             \
    if(cudaSuccess != cuErr){                                                             \
      printf("CUDA Error - %s:%d: '%s'\n", __FILE__, __LINE__, cudaGetErrorString(cuErr));\
      exit(0);                                                                            \
    }                                                                                     \
}while(0)

// Grid dimension
#define B 100

// Incrementations per block
#define T 512

// Kernel
__global__ void count(int *d_countervar){
    
    for(int i=0; i<T;i++){
    d_countervar[0] ++;
    }
}

// Main program 
int main(){

    // Device pointer for counter variable
    int *d_count;
    
    // Allocation of host counter variable
    int *countervar = (int *) malloc(sizeof(int));
    // Initialization of the counter variable
    countervar[0] = 0;
    
    // Allocation of GPU memory
    cudaErrorCheck( cudaMalloc(&d_count, sizeof(int)));
    
    // Copying the counter variable from the host to the device
    cudaErrorCheck( cudaMemcpy(d_count,countervar,sizeof(int),cudaMemcpyHostToDevice));
    
    //Launch Kernel
    count<<<B,1>>>(d_count);
    
    // Check for errors in kernel launch (e.g. invalid execution configuration paramters)
    cudaErrorCheck( cudaGetLastError());

    // Check for errors on the GPU after control is returned to CPU
    cudaErrorCheck( cudaDeviceSynchronize());
    
    // Copying the counter variable from the device to the host
    cudaErrorCheck( cudaMemcpy(countervar,d_count,sizeof(int),cudaMemcpyDeviceToHost));
    
    // Verifying result
    printf("counter: %i expected: %i \n ", countervar[0], T*B);
    
    // Freeing GPU memory
    cudaErrorCheck( cudaFree(d_count));
    
    // Freeing CPU memory
    free(countervar);
    return 0;
}