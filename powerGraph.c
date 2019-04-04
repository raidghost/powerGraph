#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "io.h"
#include "graphList.h"
#include "structs.h"
#include "tools.h"


GRAPH_LIST* genPowerGraph(GRAPH* g, unsigned int p, int supportMax)
{
	if(g == NULL)
		return NULL;
	if(supportMax == -1 || (unsigned int)supportMax > g->nbVertices)
		supportMax = g->nbVertices;

	unsigned long i,j,k;
	GRAPH_LIST* powerGraph = NULL;
	unsigned int nonZeroEntriesCurrentVertex = 0;
	unsigned long nbVerticesTotal = pow(p,g->nbVertices - 1);
	NUPLE currentVertex, vertex2Add;

	currentVertex.length = g->nbVertices - 1;
	currentVertex.tab = (unsigned int*)calloc(currentVertex.length, currentVertex.length * sizeof(unsigned int));
	vertex2Add.length = g->nbVertices;
	vertex2Add.tab = (unsigned int*)malloc(vertex2Add.length * sizeof(unsigned int));
	if(vertex2Add.tab == NULL || currentVertex.tab == NULL)
		NO_MEM_LEFT()

	/*Now we will generate all the nuple with bounded support. In the end,
	we will add a coefficient so that the sum is 1. Doing so, we will create
	every vertices of p^G thats sums to 1 and so that their support is bounded by
	supportMax.*/
	for(i = 0 ; i < nbVerticesTotal ; i++)
	{
		if(nonZeroEntriesCurrentVertex < (unsigned int)supportMax)
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

	//Now we will create all the required edges in powerGraph.
	int uG, vG;
	unsigned int nbVertices = getNbVertices(powerGraph);
	unsigned int nbSameEntries = 0;
	long sumDiff = 0;
	GRAPH_LIST *u, *v, *tmp = powerGraph;
	//We retrieve all the verticies for the graph list once for all (performances issues).
	GRAPH_LIST** vertices = (GRAPH_LIST**)malloc(nbVertices * sizeof(GRAPH_LIST*));
	if(!vertices)
		NO_MEM_LEFT()
	for(i = 0 ; tmp ; i++)
	{
		vertices[i] = tmp;
		tmp = tmp->next;
	}

	for(i = 0 ; i < nbVertices ; i++)
	{
		u = vertices[i];
		if(!u)
		{
			fprintf(stderr, "The powerGraph should have more vertices. Hence, an error happened.\n");
			exit(EXIT_FAILURE);
		}
		for(j = i+1 ; j < nbVertices ; j++)
		{
			v = vertices[j];
			if(!v)
			{
				fprintf(stderr, "The powerGraph should have more vertices. It only has %ld vertices instead of %d expected. Hence, an error happened.\n", getNbVertices(powerGraph), nbVertices);
				exit(EXIT_FAILURE);
			}
			//The vertices u and v must be equal everywhere except on two coordinates and the sum of the difference must be zero mod p.
			nbSameEntries = 0;
			sumDiff = 0;
			//This variables aims at testing if the edge we want to add correspond to an edge in G.
			uG = -1;
			vG = -1;
			for(k = 0 ; k < u->v.length ; k++)
			{
				if(u->v.tab[k] == v->v.tab[k])
					nbSameEntries++;
				else
				{
					//Beware that NUPLE.tab[k] is unsigned !
					sumDiff += (int)u->v.tab[k] - (int)v->v.tab[k];
					if(uG == -1)
						uG = k;
					else
						vG = k;
				}
			}
			if(nbSameEntries == u->v.length - 2 && sumDiff % p == 0)
			{
				//We add an edge only if uG,vG is an edge of G !
				if(g->mat[uG][vG] == 1)
					addEdgeFast(u, v);
			}
		}
	}
	free(vertices);
	return powerGraph;
}
