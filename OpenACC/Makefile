CC ?= pgcc
FLAGS = -acc -Minfo=accel

.SUFFIXES: .exe

SRC = $(wildcard *.c)
EXE = $(SRC:%.c=%.exe)

all: $(EXE)

%.exe: %.c
	$(CC) $(FLAGS) -o $@ $< $(FLAGS_OPENACC)
	
clean:
	rm -f *.exe
	
runall: $(EXE)
	for i in $(EXE) ; do echo $$i; ./$$i ; echo $$? ; done