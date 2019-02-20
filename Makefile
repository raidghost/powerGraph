all: main

main: main.o io.o display.o powerGraph.o
	gcc main.o io.o display.o powerGraph.o -lm -o main
main.o: main.c io.h display.h structs.h powerGraph.h
	gcc -c main.c -Wall -std=gnu89 -o main.o
io.o: io.c limits.h structs.h
	gcc -c io.c -Wall -std=gnu89 -o io.o
display.o: display.c structs.h
	gcc -c display.c -Wall -std=gnu89 -o display.o
powerGraph.o: powerGraph.c structs.h
	gcc -c powerGraph.c -Wall -std=gnu89 -o powerGraph.o
clean:
	rm *.o
	rm main
