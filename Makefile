OPT=-O3
#OPT=-g -O0 -fno-inline
all: main

main: main.o io.o display.o powerGraph.o tools.o rank.o
	gcc main.o io.o display.o powerGraph.o tools.o rank.o -lm -o main
main.o: main.c io.h display.h structs.h powerGraph.h tools.h
	gcc $(OPT) -c main.c -Wall -Wextra -std=gnu89 -o main.o
io.o: io.c limits.h structs.h
	gcc $(OPT) -c io.c -Wall -Wextra -std=gnu89 -o io.o
display.o: display.c structs.h
	gcc $(OPT) -c display.c -Wall -Wextra -std=gnu89 -o display.o
powerGraph.o: powerGraph.c powerGraph.h io.h structs.h limits.h tools.h rank.h display.h
	gcc $(OPT) -c powerGraph.c -Wall -Wextra -std=gnu89 -o powerGraph.o
tools.o: tools.c tools.h
	gcc $(OPT) -c tools.c -Wall -Wextra -std=gnu89 -o tools.o
rank.o: rank.c rank.h
	gcc $(OPT) -c rank.c -Wall -Wextra -std=gnu89 -lgmp -o rank.o
clean:
	rm *.o
	rm main
