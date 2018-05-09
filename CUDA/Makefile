CC = nvcc
FLAGS = -arch=sm_60

.SUFFIXES: .exe

SRC = $(wildcard *.cu)
EXE = $(SRC:%.cu=%.exe)

all: $(EXE)

DRACC_CUDA_007%.exe: DRACC_CUDA_007%.cu
	$(CC) $(FLAGS) -G -o $@ $<

%.exe: %.cu
	$(CC) $(FLAGS) -o $@ $<
	
clean:
	rm -f *.exe
	
runall: $(EXE)
	for i in $(EXE) ; do echo $$i; ./$$i ; echo $$? ; done