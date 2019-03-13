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
	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog GRAPH_FILE n [-v]\n");
		return EXIT_FAILURE;
	}
	//This variable defines the level of verbosity.
	int verbose = 0;
	if(argc >= 4)
	{
		if(strcmp(argv[3], "-v") == 0)
			verbose = 1;
		else if(strcmp(argv[3], "-vv") == 0)
			verbose = 2;
		else if(strcmp(argv[3], "-vvv") == 0)
			verbose = 3;
	}

	unsigned int n;
	GRAPH g = loadGraphFromFile(argv[1]);
	n = (unsigned int)string2Int(argv[2]);
	if(verbose >= 3)
		displayGraph(&g);
	printf("Hn est vraie jusqu'à n = %d\n", testHn(&g, n, verbose));
	freeGraph(&g);
/*	unsigned int l[] = {0,1,2,3};
	unsigned int **sub = subSequencesFixedLength(l, 4, 2);
	unsigned int i,j;
	for(i = 0 ; i < 6 ; i++)
	{
		for(j = 0 ; j < 2 ; j++)
			printf("%d ", sub[i][j]);
		free(sub[i]);
		printf("\n");
	}
	free(sub);*/
	return EXIT_SUCCESS;
}
