TARGETS = rms.o

CFLAGS=-g -pedantic -Wall -Wextra

all: $(TARGETS)

rms.o: rms.cpp rms.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f ${TARGETS} *.o *.h.gch
