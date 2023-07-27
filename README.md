# DataRaceOnAccelerator

## Introduction

DataRaceOnAccelerator or short DRACC is a microbenchmark suite designed to test the capabilities of correctness checking tools on detecting memory access issues on systems equipped with hardware accelerators. 

The generation of the executables is Makefile driven. For each individual test case a short description can be found within its source code (Located in 'Programming Model'/src/).
The microbenchmark suite is implemented in OpenMP, OpenACC and CUDA.


The naming scheme by DataRaceBench is adopted and extended for DRACC in the following way: DRACC_model_number_name_ending.c/.cu

	1. model: Used programming model (OMP for OpenMP, OACC for OpenACC and CUDA for CUDA)
	
	2. number: A three-digit number that is unique for each programming model.
	
	3. name: A name to provide a brief overview on the content of the code.
	
	4. ending: Information on the error and the desired tool behavior. The possible values of this field are:
	
	   1. yes: The correctness tool should report a defect.
	
	   2. no: The correctness tool should not report any defect.
	
	   3. other: The execution does not fail, but the correctness tool should report a defect.
	    
	    

## Usage

0. (Optional): Choose the programming model.

### Compile

1. Navigate to the folder of the chosen programming model

    
*  (OpenMP/) a Makefile for Clang is provided (recommended Clang 7.0 or higher)
* (OpenACC/) a Makefile for pgi is provided (recommended pgi 18.4 or higher)
* (CUDA/) a Makefile for nvcc is provided (recommended Cuda 9.1 or higher)
     
3. Using a different compiler is also possible, but the compiler flags might need to be adapted
4. Before compiling the code create the folder 'bin'
```shell
mkdir bin
```
5. To compile all test cases of a programming model just execute 'make' or 'make all'
```shell
make
```
```shell
make all
```
6. The executables will be generated into the folder 'bin'

### Recompiling

1. Clean the binaries by executing 'make clean'
```
make clean
```
2. Compile the test cases by executing 'make' or 'make all' 
```shell
make
```
```shell
make all
```

### Execution 
1. Individual test cases can be executed by individually by executing the generated .exe file in the bin folder
2. All test cases can be executed by typing 'make runall'
```shell
make runall
```
3. When executing with a correctness checking tool additional compiler flags might need to be added to the Makefile

  
## Coverage 

The following tables provide an overview on the coverage of the individual test cases for the defined error patterns.
Each number in the table corresponds to the number (see the naming scheme) of the test case implementing the error pattern.

![](/Error_Patterns_Between_Host_and_ACC.png)

![](/Error_Patterns_On_ACC.png)
