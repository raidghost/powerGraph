#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "io.h"
#include "display.h"
#include "structs.h"
#include "powerGraph.h"
#include "tools.h"
#include "rank.h"

int main(int argc, char *argv[])
{
	//We initiate the pseudo random generator (used in quicksort)
	srand(time(NULL));
	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog [GRAPH_FILE] [n]\n");
		return EXIT_FAILURE;
	}
	unsigned int n;
	GRAPH g;
	g = loadGraphFromFile(argv[1]);
	n = string2Int(argv[2]);
	displayGraph(&g);
	printf("n = %d\n", n);
	printf("Hn est vraie jusqu'à n = %d\n", testHn(&g, n));
	freeGraph(&g);
	return EXIT_SUCCESS;
}
