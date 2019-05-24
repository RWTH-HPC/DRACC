CC = nvcc
FLAGS = -arch=sm_60

.SUFFIXES: .exe

SRC = $(wildcard src/*.cu)
EXE = $(SRC:src/%.cu=bin/%.exe)

all: $(EXE)

DRACC_CUDA_007%.exe: DRACC_CUDA_007%.cu
	$(CC) $(FLAGS) -G -o $@ $<

bin/%.exe: src/%.cu
	$(CC) $(FLAGS) -o $@ $<
	
clean:
	rm -f bin/*.exe
	
runall: $(EXE)
	for i in $(EXE) ; do echo $$i; ./$$i ; echo $$? ; done
