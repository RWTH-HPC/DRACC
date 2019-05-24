/*
Incrementaion with a threshold of N. The incrementation is atomic, but the query on the threshold depends on the counter.
The query is not atomic, therefore a Data Race is possible in line 30 and 31.
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
#define B 128

// Block dimension
#define T 512

// Count iterations
#define N 200000


// Kernel
__global__ void count( int *d_countervar){
    for(int i=0; i<N*2; i++){
        if(d_countervar[0]<N){
            atomicAdd(d_countervar,1);
        }
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
    count<<<B,T>>>(d_count);
    
     // Check for errors in kernel launch (e.g. invalid execution configuration paramters)
    cudaErrorCheck( cudaGetLastError());

    // Check for errors on the GPU after control is returned to CPU
    cudaErrorCheck( cudaDeviceSynchronize());
    
    // Copying the counter variable from the device to the host
    cudaErrorCheck( cudaMemcpy(countervar,d_count,sizeof(int),cudaMemcpyDeviceToHost));
    
    // Verifying result
    printf("counter: %i expected: %i \n ", countervar[0], N);
    
    // Freeing GPU memory
    cudaErrorCheck( cudaFree(d_count));
    
    // Freeing CPU memory
    free(countervar);
    return 0;
}