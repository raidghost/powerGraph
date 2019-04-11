#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "homology.h"
#include "io.h"
#include "structs.h"

#define NB_VERTICES_MAX 10000

GRAPH createRandomGraph(const int nbVertices, const long double p, const bool connected)
{
	/*If nbVertices == -1 then we choose the number of vertices uniformly at random between 0 and NB_VERTICES_MAX.
	p is the probability for an edge to exists. All graphs are simple and non oriented.*/
	int randomMax = (int)floor(p * RAND_MAX);
	unsigned long i,j;
	GRAPH g;

	if(nbVertices < 0)
		g.nbVertices = rand() % (NB_VERTICES_MAX + 1);
	else
		g.nbVertices = nbVertices;

	g.mat = (char**)malloc(g.nbVertices * sizeof(char*));
	if(!g.mat)
		NO_MEM_LEFT()
	for(i = 0 ; i < g.nbVertices ; i++)
	{
		g.mat[i] = (char*)malloc(g.nbVertices * sizeof(char));
		if(!g.mat)
			NO_MEM_LEFT()
	}

	do
	{
		for(i = 0 ; i < g.nbVertices ; i++)
		{
			g.mat[i][i] = 0;
			for(j = i+1 ; j < g.nbVertices ; j++)
			{
				if(rand() <= randomMax)
				{
					g.mat[i][j] = 1;
					g.mat[j][i] = 1;
				}
				else
				{
					g.mat[i][j] = 0;
					g.mat[j][i] = 0;
				}
			}
		}
		if(!connected)
			return g;
	}while(!isGraphConnected(&g));
	return g;
}

