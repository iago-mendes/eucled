CFLAGS=-g -pedantic -Wall -Wextra

binary: ../../algorithms/bicgstab.cpp ../../algorithms/bicgstab.h ../../algorithms/factorization.cpp ../../algorithms/factorization.h ../../algorithms/integration.cpp ../../algorithms/integration.h ../../algorithms/relaxation.cpp ../../algorithms/relaxation.h ../../classes/dyad.cpp ../../classes/dyad.h ../../classes/grid.cpp ../../classes/grid.h ../../classes/metric.cpp ../../classes/metric.h ../../classes/time_stepper.cpp ../../classes/time_stepper.h ../../functions/commutator.cpp ../../functions/commutator.h ../../functions/cross_product.cpp ../../functions/cross_product.h ../../utils/dot_product.cpp ../../utils/dot_product.h ../../utils/rms.cpp ../../utils/rms.h main.o
	cd ../../algorithms; make; cd ../exercises/07-factorization
	cd ../../classes; make; cd ../exercises/07-factorization
	cd ../../functions; make; cd ../exercises/06-relaxation
	cd ../../utils; make; cd ../exercises/07-factorization
	g++ $(CFLAGS) -o $@ ../../algorithms/bicgstab.o ../../algorithms/factorization.o ../../algorithms/integration.o ../../algorithms/relaxation.o ../../classes/dyad.o ../../classes/grid.o ../../classes/metric.o ../../classes/time_stepper.o ../../functions/commutator.o ../../functions/cross_product.o ../../utils/dot_product.o ../../utils/rms.o main.o

main.o: main.cpp main.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f binary *.o *.h.gch


