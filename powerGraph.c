#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "tools.h"
#include "io.h"

void nupleReset(const NUPLE *nuple)
{
	unsigned int i;
	for(i = 0 ; i < nuple->length ; i++)
		nuple->tab[i] = 0;
}

void padiquePP(const NUPLE *nuple, const unsigned int basis)
{//Add 1 to a number in basis "basis".
	if(basis == 0)
		return;
	unsigned int k = 0;
	for(k = 0 ; k < nuple->length && nuple->tab[k] == basis ; k++);
	if(k == nuple->length)
		nupleReset(nuple);
	else
		nuple->tab[k]++;
}

GRAPH genPowerGraph(GRAPH *g, unsigned int p, unsigned int supportMax)
{//Generate the graph n^g (which mean modulo n and not in the field with n elements (if such a field exists...).
	if(g->nbVertices == 0)
		exit(EXIT_FAILURE);
	if(supportMax > g->nbVertices || supportMax == 0)
		supportMax = g->nbVertices;
	unsigned long i,j,k,l;
	unsigned long power = 1;
	unsigned long** binom = binomAll(g->nbVertices);
	GRAPH powerGraph;
	NUPLE nuple;

	powerGraph.nbVertices = binom[g->nbVertices][supportMax] * (unsigned long)pow((double)p,(double)(supportMax - 1));
	//We initiate a nuple that will be useful next.
	nuple.length = supportMax - 1;
	nupleReset(&nuple);

	//Initialization of the powerGraph
	powerGraph.mat = (char**)malloc(powerGraph.nbVertices * sizeof(char*));
	if(powerGraph.mat == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		powerGraph.mat[i] = (char*)calloc(powerGraph.nbVertices, powerGraph.nbVertices * sizeof(char));
		if(powerGraph.mat[i] == NULL)
			NO_MEM_LEFT()
	}
	
	//We choose k positions.
	for(k = 0 ; k < supportMax ; k++)
	{
		//We choose k values.
		for(j = 0 ; j < binom[supportMax][k] ; j++)
		{
			for(i = 0 ; i < power - 1 ; i++)
			{
				for(l = 0 ; l < k ; l++)
					powerGraph.mat[j][l] = 1;
			}
		}

	}

	//We have no choice for the last value since the sum of the coefficients must be 1.
	
	return powerGraph;
}


