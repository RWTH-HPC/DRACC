/*
Array with backwards dependencies. Order Violation. Data Race in line 38. Intra Region Data Race.
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

// Iterations per thread
#define B 100

// Block dimension
#define T 512

// Host pointer
int *countervar;

// Initialization
int init(){
    for(int i=0; i<B*T; i++){
        countervar[i]=0;
    }
    return 0;
}

// Kernel
__global__ void count(int *countervar){
    int i;
    for(i = threadIdx.x * B; i < threadIdx.x * B + B; i++){
        if(i!=0){
            countervar[i] = countervar[i-1] + 1;
        }
    }
}

// Verifying result
int check(){
    bool test = false;
    for(int i=0; i<B*T; i++){
        if(countervar[i]!=i){
            test = true;
        }
    }
    printf("Memory Access Issue visible: %s\n",test ? "true" : "false");
    return 0;
}

// Main program 
int main(){

    // Device pointer for counter variable
    int *d_count;
    
    // Allocation of host counter variable
    countervar = (int *) malloc(B*T*sizeof(int));
    // Initialization of the counter variable
    init();
    
    // Allocation of GPU memory
    cudaErrorCheck( cudaMalloc(&d_count, B*T*sizeof(int)));
    
    // Copying the counter variable from the host to the device
    cudaErrorCheck( cudaMemcpy(d_count,countervar,B*T*sizeof(int),cudaMemcpyHostToDevice));
    
    //Launch Kernel
    count<<<1,T>>>(d_count);
    
     // Check for errors in kernel launch (e.g. invalid execution configuration paramters)
    cudaErrorCheck( cudaGetLastError());

    // Check for errors on the GPU after control is returned to CPU
    cudaErrorCheck( cudaDeviceSynchronize());
    
    // Copying the counter variable from the device to the host
    cudaErrorCheck( cudaMemcpy(countervar,d_count,B*T*sizeof(int),cudaMemcpyDeviceToHost));
    
    // Verifying result
    check();
    
    // Freeing GPU memory
    cudaErrorCheck( cudaFree(d_count));
    
    // Freeing CPU memory
    free(countervar);
    return 0;
}
