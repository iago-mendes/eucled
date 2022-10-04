CFLAGS=-g -pedantic -Wall -Wextra

binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../utils/rms.cpp ../../utils/rms.h main.cpp main.h
	g++ $(CFLAGS) -o $@ $^

clean:
	rm -f binary *.o
