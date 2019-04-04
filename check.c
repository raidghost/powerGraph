/*This program aims at testing the main program.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "io.h"
#include "structs.h"
#include "tools.h"

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog GRAPH_FILE nbVertices [nbEdges] [commonDegree]\n");
		return EXIT_FAILURE;
	}
	GRAPH g = loadGraphFromFile(argv[1]);
	unsigned int nbVerticesExpected = (unsigned int)string2Int(argv[2]);
	bool checkNbEdges = false, checkCommonDegree = false;
	unsigned int nbEdgesExpected = 0;
	int commonDegreeExpected = 0;
	if(argc >= 4)
	{
		checkNbEdges = true;
		nbEdgesExpected = (unsigned int)string2Int(argv[3]);
	}
	if(argc >= 5)
	{
		checkCommonDegree = true;
		commonDegreeExpected = string2Int(argv[4]);
	}

	//Tests
	if(g.nbVertices != nbVerticesExpected)
		printf("Error : The graph \"%s\" has %ld vertices instead of %d\n", argv[1], g.nbVertices, nbVerticesExpected);
	else
		printf("The number of vertices is OK.\n");
	if(checkNbEdges)
	{
		unsigned int nbEdgesG = nbEdges(&g);
		if(nbEdgesExpected != nbEdgesG)
			printf("Error: The graph \"%s\" has %d edges instead of %d\n", argv[1], nbEdgesG, nbEdgesExpected);
		else
			printf("The number of edges is OK.\n");
	}
	if(checkCommonDegree)
	{
		int commonDegreeG = commonDegree(&g);
		if(commonDegreeG == -1)
			printf("The graph \"%s\" has no common degree.\n", argv[1]);
		else if(commonDegreeG != commonDegreeExpected)
			printf("Error: The graph \"%s\" has common degree %d instead of %d.\n", argv[1], commonDegreeG, commonDegreeExpected);
		else
			printf("The common degree is OK.\n");
	}
	return EXIT_SUCCESS;
}

unsigned int nbEdges(GRAPH *g)
{
	unsigned int nbEdges = 0;
	unsigned int i,j;
	
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j] == 1)
				nbEdges++;
		}
	}
	return nbEdges / 2;
}

int commonDegree(GRAPH *g)
{//This function test whether all the vertices of G have the same degree. If so, it returns this common degree and if not it return -1
	unsigned long i,j;
	int commonDegree = 0, degreeTmp;
	if(g->nbVertices == 0)
		return -1;
	//First we initialize commonDegree.
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		if(g->mat[i][0] == 1)
			commonDegree++;
	}
	//Now we try to find some vertex with another degree.
	for(i = 1 ; i < g->nbVertices ; i++)
	{
		degreeTmp = 0;
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j] == 1)
				degreeTmp++;
		}
		if(degreeTmp != commonDegree)
			return -1;
	}
	return commonDegree;
}
