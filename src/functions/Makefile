TARGETS = commutator.o cross_product.o dot_product.o

CFLAGS=-g -pedantic -Wall -Wextra

all: $(TARGETS)

commutator.o: commutator.cpp commutator.h
	g++ $(CFLAGS) -c $^

cross_product.o: cross_product.cpp cross_product.h
	g++ $(CFLAGS) -c $^

dot_product.o: dot_product.cpp dot_product.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f ${TARGETS} *.o *.h.gch
