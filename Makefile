all: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 -o gameoflife gameoflife.o -lgol -L.

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c gol.c

gameoflife.o: gameoflife.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c gameoflife.c

libgol.so: gol.o gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -shared -o libgol.so gol.o

clean:
	rm -rf gameoflife gameoflife.o gol.o libgol.so
