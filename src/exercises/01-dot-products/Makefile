CFLAGS=-g -pedantic -Wall -Wextra

# binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../utils/math.h ../../utils/colors.h main.cpp main.h
# 	g++ $(CFLAGS) -o $@ $^

binary: ../../classes/grid.cpp ../../classes/grid.h ../../classes/dyad.cpp ../../classes/dyad.h ../../classes/metric.cpp ../../classes/metric.h ../../utils/math.h ../../utils/colors.h ../../utils/rms.cpp ../../utils/rms.h main.o
	cd ../../classes; make; cd ../exercises/01-dot-products
	cd ../../utils; make; cd ../exercises/01-dot-products
	g++ $(CFLAGS) -o $@ ../../classes/grid.o ../../classes/dyad.o ../../classes/metric.o ../../utils/math.h ../../utils/colors.h ../../utils/rms.o main.o

main.o: main.cpp main.h
	g++ $(CFLAGS) -c $^

clean:
	rm -f binary *.o *.gch
