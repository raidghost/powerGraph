#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "display.h"
#include "io.h"
#include "graphList.h"
#include "powerGraph.h"
#include "structs.h"
#include "tools.h"


GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, unsigned int supportMax)
{
	if(g == NULL)
		return NULL;
	if(supportMax > g->nbVertices)
		supportMax = g->nbVertices;

	unsigned long i,j,k,l;
	unsigned long nbVertices = 0;
	GRAPH_LIST* powerGraph = NULL;
	unsigned int nonZeroEntriesCurrentVertex = 0;
	unsigned long nbVerticesTotal = pow(p,g->nbVertices - 1);
	NUPLE currentVertex, vertex2Add;

	currentVertex.length = g->nbVertices - 1;
	currentVertex.tab = (unsigned int*)calloc(currentVertex.length, currentVertex.length * sizeof(unsigned int));
	vertex2Add.tab = (unsigned int*)malloc(vertex2Add.length * sizeof(unsigned int));
	if(vertex2Add.tab == NULL)
		NO_MEM_LEFT()

	if(currentVertex.tab == NULL)
		NO_MEM_LEFT()

	vertex2Add.length = g->nbVertices;
	for(i = 0 ; i < nbVerticesTotal ; i++)
	{
		//displayNuple(&currentVertex);
		//printf("\n");
		if(nonZeroEntriesCurrentVertex < supportMax)
		{
			for(j = 0 ; j <= currentVertex.length ; j++)
			{
				unsigned int sumCurrentVertex = 0;
				for(k = 0 ; k < j ; k++)
				{
					vertex2Add.tab[k] = currentVertex.tab[k];
					sumCurrentVertex += currentVertex.tab[k];
				}
				for(k = j+1 ; k < vertex2Add.length ; k++)
				{
					vertex2Add.tab[k] = currentVertex.tab[k-1];
					sumCurrentVertex += currentVertex.tab[k-1];
				}
				//We consider the connected component of p^G that sums to 1 (mod p).
				vertex2Add.tab[j] = (p+1 - sumCurrentVertex % p) % p;
				powerGraph = addVertex(powerGraph, &vertex2Add);
			}
		}
		//We add 1 to currentVertex.
		for(j = 0 ; j < currentVertex.length && currentVertex.tab[j] == p-1 ; j++);
		if(j < currentVertex.length)
		{
			if(currentVertex.tab[j] == 0)
				nonZeroEntriesCurrentVertex++;
			currentVertex.tab[j]++;
		}
		for(k = 0 ; k < j ; k++)
		{
			currentVertex.tab[k] = 0;
			nonZeroEntriesCurrentVertex--;
		}
	}

	free(currentVertex.tab);
	free(vertex2Add.tab);
	return powerGraph;
}
