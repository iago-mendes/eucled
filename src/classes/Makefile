TARGETS = dyad.o grid.o

CFLAGS=-g -pedantic -Wall -Wextra

all: $(TARGETS)

dyad.o: dyad.cpp dyad.h
	g++ $(CFLAGS) -c $^

grid.o: grid.cpp grid.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f ${TARGETS} *.o *.h.gch
