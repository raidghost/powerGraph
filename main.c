#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "display.h"
#include "structs.h"
#include "powerGraph.h"
#include "tools.h"

int main(int argc, char *argv[])
{
	//We initiate the pseudo random generator (used in quicksort)
	srand(time(NULL));
	if(argc < 2)
	{
		fprintf(stderr, "Usage : prog [GRAPH_FILE]\n");
		return EXIT_FAILURE;
	}
	GRAPH g;
	g = loadGraphFromFile(argv[1]);
	displayGraph(&g);
	testHn(&g, 2, 3);
	freeGraph(&g);
	return EXIT_SUCCESS;
}
