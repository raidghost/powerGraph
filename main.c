#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	if(argc < 5)
	{
		fprintf(stderr, "Usage : prog GRAPH_FILE n field [-v]\nPossible values for field are : F2, R\n");
		return EXIT_FAILURE;
	}
	//Variable to handle verbosity.
	int verbose = 0;
	if(argc >= 5)
	{
		if(strcmp(argv[4], "-v") == 0)
			verbose = 1;
		else if(strcmp(argv[4], "-vv") == 0)
			verbose = 2;
		else if(strcmp(argv[4], "-vvv") == 0)
			verbose = 3;
	}

	unsigned int n;
	int field;

	GRAPH g = loadGraphFromFile(argv[1]);
	n = (unsigned int)string2Int(argv[2]);

	if(strcmp(argv[3], "F2") == 0)
		field = 2;
	else if(strcmp(argv[3], "R") == 0)
		field = 0;
	else
	{
		fprintf(stdout, "Possible values for field are : F2, R\nWe use R by default.");
		field = 0;
	}

	if(verbose >= 3)
		displayGraph(&g);
	printf("Hn est vraie jusqu'à n = %d\n", testHn(&g, n, field, verbose));
	freeGraph(&g);
	return EXIT_SUCCESS;
}
