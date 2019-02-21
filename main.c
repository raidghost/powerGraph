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
	unsigned int t[] = {0,1,2,3};
	subSequences(t, 4);
	return EXIT_SUCCESS;
}
