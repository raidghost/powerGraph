#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "display.h"
#include "structs.h"
#include "powerGraph.h"

int main(int argc, char *argv[]){
/*	GRAPH g;
	g = loadGraphFromFile("graph");
	displayGraph(g);
	freeGraph(g);*/
	PADIQUE bob;
	bob = padique(7,3);
	return EXIT_SUCCESS;
}
