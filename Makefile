CPP = gcc
FLAGS = 

all: clean exec manager.o

exec: executor.c so_lib.c
	$(CPP) $(FLAGS) executor.c so_lib.c -o exec

manager.o: manager.c so_lib.c
	$(CPP) $(FLAGS) manager.c so_lib.c

clean:
	rm -f *.o exec

.PHONY: all clean
