TARGETS = factorization.o bicgstab.o integration.o

CFLAGS=-g -pedantic -Wall -Wextra

all: $(TARGETS)

factorization.o: factorization.cpp factorization.h
	g++ $(CFLAGS) -c $^

bicgstab.o: bicgstab.cpp bicgstab.h
	g++ $(CFLAGS) -c $^

integration.o: integration.cpp integration.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f ${TARGETS} *.o *.h.gch
