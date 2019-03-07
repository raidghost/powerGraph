#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "display.h"
#include "structs.h"
#include "powerGraph.h"
#include "tools.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage : prog [GRAPH_FILE]\n");
		return EXIT_FAILURE;
	}
	GRAPH g;
	g = loadGraphFromFile(argv[1]);
	displayGraph(&g);
	generateDn(&g, 2);
	freeGraph(&g);
	return EXIT_SUCCESS;
}
