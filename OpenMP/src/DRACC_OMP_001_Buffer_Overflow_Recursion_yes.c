/*
Causing a buffer overflow on the accelerator with recursion in line 15.
*/

#define C 1024


#pragma omp declare target
int counter = 0;
int rekurs(){    
    for(int i = 0; i<C; i++){
        #pragma omp atomic
        counter++;

        rekurs();        
    }
    return 0;
}
#pragma omp end declare target




int main(){
    #pragma omp target map(tofrom:counter) device(0)
    rekurs();
    return 0;
}