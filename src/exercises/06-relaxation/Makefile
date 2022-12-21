CFLAGS=-g -pedantic -Wall -Wextra

binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../classes/time_stepper.cpp ../../classes/time_stepper.h ../../utils/rms.cpp ../../utils/rms.h ../../functions/commutator.cpp ../../functions/commutator.h ../../functions/cross_product.cpp ../../functions/cross_product.h ../../algorithms/relaxation.cpp ../../algorithms/relaxation.h main.o
	cd ../../classes; make; cd ../exercises/06-relaxation
	cd ../../utils; make; cd ../exercises/06-relaxation
	cd ../../functions; make; cd ../exercises/06-relaxation
	g++ $(CFLAGS) -o $@ ../../classes/grid.o ../../classes/dyad.o ../../classes/time_stepper.o ../../utils/rms.o ../../functions/commutator.o ../../functions/cross_product.o ../../algorithms/relaxation.o main.o

main.o: main.cpp main.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f binary *.o *.h.gch
