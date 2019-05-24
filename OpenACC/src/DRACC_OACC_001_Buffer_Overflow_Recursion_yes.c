/*
Causing a buffer overflow on the accelerator with recursion in line 15.
OpenACC can not compile endless recursions, therefore this code will not compile as of OpenACC 2.6.
*/

#define C 1024



int counter = 0;
int rekurs(){    
    for(int i = 0; i<C; i++){
        #pragma acc atomic update
        counter++;        
        rekurs();        
    }
    return 0;
}





int main(){
    #pragma acc parallel
    rekurs();
    return 0;
}