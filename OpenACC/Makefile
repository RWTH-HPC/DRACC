CC ?= pgcc
FLAGS ?= -acc -Minfo=accel

.SUFFIXES: .exe

SRC = $(wildcard src/*.c)
EXE = $(SRC:src/%.c=bin/%.exe)

all: $(EXE)

bin/%.exe: src/%.c
	$(CC) $(FLAGS) -o $@ $< $(FLAGS_OPENACC)
	
clean:
	rm -f *.exe
	
runall: $(EXE)
	for i in $(EXE) ; do echo $$i; ./$$i ; echo $$? ; done
