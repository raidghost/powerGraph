#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "io.h"
#include "display.h"
#include "structs.h"
#include "homology.h"
#include "tools.h"
#include "rank.h"

#include "powerGraph.h"
#include "graphList.h"

//GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, unsigned int supportMax);
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
//	printf("Hn est vraie jusqu'à n = %d\n", testHn(&g, n, field, verbose));
	GRAPH_LIST *graphL = NULL;
	graphL = genPowerGraph(&g, 3, 3);
	displayGraphList(graphL);
	freeGraph(&g);
/*	NUPLE u,v,w;
	v.length = 3;
	v.tab = (unsigned int*)malloc(3 * sizeof(unsigned int));
	v.tab[0] = 1;
	v.tab[1] = 2;
	v.tab[2] = 3;
	GRAPH_LIST *g = NULL;
	g = addVertex(g, v);
	u.length = 3;
	u.tab = (unsigned int*)malloc(3 * sizeof(unsigned int));
	u.tab[0] = 3;
	u.tab[1] = 1;
	u.tab[2] = 1;

	w.length = 3;
	w.tab = (unsigned int*)malloc(3 * sizeof(unsigned int));
	w.tab[0] = 1;
	w.tab[1] = 2;
	w.tab[2] = 4;

	g = addVertex(g,u);
	g = addVertex(g,w);
	displayGraphList(g);
	if(searchVertex(g, w))
		printf("On a trouvé le sommet à l'adresse %p\n", searchVertex(g, w));
	else
		printf("bug");*/

	freeGraphList(graphL);
	return EXIT_SUCCESS;
}
