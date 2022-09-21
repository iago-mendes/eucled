CFLAGS=-g -pedantic -Wall -Wextra

binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../utils/math.h ../../utils/colors.h main.cpp main.h
	g++ $(CFLAGS) -o $@ $^

clean:
	rm -f binary *.o
