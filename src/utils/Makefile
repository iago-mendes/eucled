TARGETS = rms.o dot_product.o

CFLAGS=-g -pedantic -Wall -Wextra

all: $(TARGETS)

rms.o: rms.cpp rms.h
	g++ $(CFLAGS) -c $^

dot_product.o: dot_product.cpp dot_product.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f ${TARGETS} *.o *.h.gch
