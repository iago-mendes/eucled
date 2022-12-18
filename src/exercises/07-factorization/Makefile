CFLAGS=-g -pedantic -Wall -Wextra

binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../classes/metric.cpp ../../classes/metric.h ../../algorithms/factorization.cpp ../../algorithms/factorization.h ../../utils/rms.cpp ../../utils/rms.h main.o
	cd ../../classes; make; cd ../exercises/07-factorization
	cd ../../utils; make; cd ../exercises/07-factorization
	cd ../../algorithms; make; cd ../exercises/07-factorization
	g++ $(CFLAGS) -o $@ ../../classes/grid.o ../../classes/dyad.o ../../classes/metric.o ../../algorithms/factorization.o ../../utils/rms.o main.o

main.o: main.cpp main.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f binary *.o *.h.gch
