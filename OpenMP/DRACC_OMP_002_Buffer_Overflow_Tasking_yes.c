/*
Causing a buffer overflow on the accelerator with recursive tasks in line 17.
*/
#include <stdio.h>
#define C 1024



#pragma omp declare target
int counter = 0;

int rekurs(){
    for(int i = 0; i<C; i++){
        #pragma omp atomic
        counter++;
        #pragma omp task
        rekurs();
        
        
    }

  return counter;
}
#pragma omp end declare target




int main(){
    #pragma omp target map(tofrom:counter) device(0)
    rekurs();
    printf("counter %i\n",counter);
    return 0;
}