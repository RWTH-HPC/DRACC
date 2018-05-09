CC ?= clang
FLAGS = 
FLAGS_OPENMP = -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda

.SUFFIXES: .exe

SRC = $(wildcard *.c)
EXE = $(SRC:%.c=%.exe)

all: $(EXE)

%.exe: %.c
	$(CC) $(FLAGS) -o $@ $< $(FLAGS_OPENMP)
	
clean:
	rm -f *.exe
	
runall: $(EXE)
	for i in $(EXE) ; do echo $$i; ./$$i ; echo $$? ; done