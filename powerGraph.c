#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "io.h"
#include "graphList.h"
#include "structs.h"
#include "tools.h"
#include "rank.h"

#include "display.h"


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

EK_CERT ekCert(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p, int field)
{
	/*An edge clique is a list of list of nuple. Each nuple represent a vertex.
	Each list of nuples must be of size p.*/
	if(p < 1)
	{
		fprintf(stderr, "We cannot look for edge cliques in 0^G !\n");
		exit(EXIT_FAILURE);
	}
	int* positionNuple;
	unsigned int nbVerticesPG = getNbVertices(powerGraph), currentEK = 0;
	unsigned long i,j,k,l,m;
	NUPLE u;
	NUPLE *listVerticesPGOrdered, *newEk;
	bool isEkCompletelyNew;//If an ek contains at least one vertex already in an other ek this boolean must be set to false.
	bool* listVerticesPGOrderedBool;//Contains true iff we have already pu vertex number k in an ek.
	GRAPH_LIST* tmp;
	EK_CERT ekCert;

	//We know exactly how many edge clique there is in the graph.
	ekCert.nbEk = nbVerticesPG / p;
	ekCert.nbEltPerEk = p;
	ekCert.ek = (NUPLE**)malloc(ekCert.nbEk * sizeof(NUPLE*));
	ekCert.weight = (long double*)malloc(ekCert.nbEk * sizeof(long double));
	if(!ekCert.ek || !ekCert.weight)
		NO_MEM_LEFT()
	for(i = 0 ; i < ekCert.nbEk ; i++)
	{
		ekCert.ek[i] = (NUPLE*)malloc(ekCert.nbEltPerEk * sizeof(NUPLE));
		if(!ekCert.ek[i])
			NO_MEM_LEFT()
		for(j = 0 ; j < ekCert.nbEltPerEk ; j++)
		{
			ekCert.ek[i][j].length = ekCert.nbEltPerEk;
			ekCert.ek[i][j].tab = (unsigned int*)malloc(ekCert.nbEltPerEk * sizeof(unsigned int));
			if(!ekCert.ek[i][j].tab)
				NO_MEM_LEFT()
		}
	}

	//We retreive all the vertices in an array (fast access). We assume g to be ordered.
	listVerticesPGOrdered = (NUPLE*)malloc(nbVerticesPG * sizeof(NUPLE));
	newEk = (NUPLE*)malloc(p * sizeof(NUPLE));
	listVerticesPGOrderedBool = (bool*)malloc(nbVerticesPG * sizeof(bool));
	positionNuple = (int*)malloc(ekCert.nbEltPerEk * sizeof(int));
	u.length = g->nbVertices;
	u.tab = (unsigned int*)malloc(u.length * sizeof(unsigned int));
	if(!listVerticesPGOrdered || !newEk || !listVerticesPGOrderedBool || !positionNuple || !u.tab)
		NO_MEM_LEFT()
	tmp = powerGraph;
	for(i = 0 ; i < nbVerticesPG ; i++)
	{
		listVerticesPGOrdered[i] = tmp->v;
		//This means that at the begening we have never encountered vertex number i.
		listVerticesPGOrderedBool[i] = false;
		tmp = tmp->next;
	}

	//----------------- This part could be improved.
	for(l = 0 ; l < nbVerticesPG ; l++)
	{
		if(!listVerticesPGOrderedBool[l])
		{//If this is the first time we see this vertex.
			positionNuple[0] = l;
			nupleCpy(listVerticesPGOrdered + l, &u);
			nupleCpy(&u, newEk);
			for(i = 0 ; i < g->nbVertices ; i++)
			{
				for(j = i+1 ; j < g->nbVertices ; j++)
				{
					if(g->mat[i][j] == 1)
					{//We generate the edge clique directed by the edge i,j and containing u.
						isEkCompletelyNew = true;
						for(k = 1 ; k < p ; k++)
						{//We generate the other elements of the current ek.
							u.tab[i] = (u.tab[i] + 1) % p;
							u.tab[j] = (u.tab[j] + p-1) % p;
							positionNuple[k] = dichoSearchNupleList(listVerticesPGOrdered, &u, 0, nbVerticesPG - 1);
							if(positionNuple[k] == -1 || listVerticesPGOrderedBool[positionNuple[k]])
							{//This means either we did not generate the whole powerGraph, either we have already an ek containning the current vertex.
								isEkCompletelyNew = false;
								break;
							}
						}
						if(isEkCompletelyNew)
						{
							for(k = 0 ; k < p ; k++)
							{
								nupleCpy(newEk + k, ekCert.ek[currentEK] + k);
								listVerticesPGOrderedBool[positionNuple[k]] = true;
							}
							currentEK++;
							i = g->nbVertices;
							j = g->nbVertices;
						}
						else
						{//If the ek is not compeletely new then we discard all the vertices we have seen during this round.
							for(m = 0 ; m <= k ; m++)
							{
								if(positionNuple[k] != -1)
									listVerticesPGOrderedBool[positionNuple[k]] = false;
							}
						}
						printf("\n");
						for(k = 0 ; k < nbVerticesPG ; k++)
						{
							if(listVerticesPGOrderedBool[k])
								printf("1-");
							else
								printf("0-");
						}
					}
				}
			}
		}
	}

	displayGraphList(powerGraph);
	printf("\n\n");
	displayEkCert(&ekCert);
	//We free useless memory.
	free(newEk);
	free(listVerticesPGOrderedBool);
	free(positionNuple);
	free(u.tab);

	if(field == 2)
	{//If the field is F2
		MATRIX_F2 ekVSVertices;
		ekVSVertices.nbRows = ekCert.nbEk;
		ekVSVertices.nbColumns = nbVerticesPG;

		ekVSVertices.mat = (char**)malloc(ekVSVertices.nbRows * sizeof(char*));
		if(!ekVSVertices.mat)
			NO_MEM_LEFT()
		for(i = 0 ; i < ekCert.nbEk ; i++)
		{
			ekVSVertices.mat[i] = (char*)calloc(ekVSVertices.nbColumns, ekVSVertices.nbColumns * sizeof(char));
			if(!ekVSVertices.mat[i])
				NO_MEM_LEFT()
			for(k = 0 ; k < ekCert.nbEltPerEk ; k++)
				ekVSVertices.mat[i][dichoSearchNupleList(listVerticesPGOrdered, &ekCert.ek[i][k], 0, nbVerticesPG - 1)] = 1;
		}
		displayMatrixF2(&ekVSVertices);
		printf("Le rang vaut %ld.\n", rankF2(&ekVSVertices));

		//We free useless memory.
		for(i = 0 ; i < ekCert.nbEk ; i++)
			free(ekVSVertices.mat[i]);
		free(ekVSVertices.mat);
	}

	free(listVerticesPGOrdered);
	return ekCert;
}
