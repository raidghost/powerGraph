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

EK_CERT findEkCert(GRAPH* g, GRAPH_LIST* powerGraph, unsigned int p, int field)
{
	/*An edge clique is a list of list of nuple. Each nuple represent a vertex.
	Each list of nuples must be of size p.*/
	if(p < 1)
	{
		fprintf(stderr, "We cannot look for edge cliques in 0^G !\n");
		exit(EXIT_FAILURE);
	}
	int nbEdgesG;
	int* positionNuple;
	unsigned int nbVerticesPG = getNbVertices(powerGraph);
	unsigned long i,j,k,l,m;
	NUPLE *listVerticesPGOrdered, *newEk;
	bool addEk;
	bool *listVerticesPGOrderedBool, *newEkBool;
	GRAPH_LIST* tmp;
	EK_CERT ekCert;

	ekCert.nbEk = 0;
	ekCert.nbEltPerEk = p;

	positionNuple = (int*)malloc(p * sizeof(int));
	listVerticesPGOrdered = (NUPLE*)malloc(nbVerticesPG * sizeof(NUPLE));
	newEk = (NUPLE*)malloc(p * sizeof(NUPLE));
	listVerticesPGOrderedBool = (bool*)malloc(nbVerticesPG * sizeof(bool));
	newEkBool = (bool*)malloc(p * sizeof(bool));
	//We do not know the exact number of edge cliques so we consider an upper bound. It will be reallocated at the end of the procedure.
	nbEdgesG = nbEdges(g);
	ekCert.ek = (NUPLE**)malloc(nbVerticesPG * nbEdgesG * sizeof(NUPLE*));

	if(!positionNuple || !listVerticesPGOrdered || !newEk || !listVerticesPGOrderedBool || !newEkBool || ! ekCert.ek)
		NO_MEM_LEFT()
	for(k = 0 ; k < p ; k++)
	{
		newEk[k].tab = (unsigned int*)malloc(g->nbVertices * sizeof(unsigned int));
		if(!newEk[k].tab)
			NO_MEM_LEFT()
		newEk[k].length = g->nbVertices;
	}

	tmp = powerGraph;
	for(i = 0 ; i < nbVerticesPG ; i++)
	{
		listVerticesPGOrdered[i] = tmp->v;
		//This means that at the begening we have never encountered vertex number i.
		listVerticesPGOrderedBool[i] = false;
		tmp = tmp->next;
	}

	for(l = 0 ; l < nbVerticesPG ; l++)
	{
		for(i = 0 ; i < g->nbVertices ; i++)
		{
			for(j = i+1 ; j < g->nbVertices ; j++)
			{
				if(g->mat[i][j] == 1)
				{
					nupleCpy(listVerticesPGOrdered + l, newEk);
					positionNuple[0] = l;
					addEk = true;
					for(k = 1 ; k < p ; k++)
					{
						nupleCpy(newEk + k-1, newEk + k);
						newEk[k].tab[i] = (newEk[k].tab[i] + 1) % p;
						newEk[k].tab[j] = (newEk[k].tab[j] + p-1) % p;
						positionNuple[k] = dichoSearchNupleList(listVerticesPGOrdered, newEk + k, 0, nbVerticesPG - 1);
						if(positionNuple[k] == -1)
						{//This means we have created a vertex that does not exists in powerGraph. Is is possible if the support of powerGraph has been bounded.
							addEk = false;
							break;
						}
					}
					if(addEk)
					{
						//First, we check if the newEk has already been found (the order of the elements does not count).
						for(m = 0 ; m < ekCert.nbEk ; m++)
						{
							unsigned int nbEltAlreadySeen = 0;
							for(k = 0 ; k < p ; k++)
							{
								if(nupleCmp(ekCert.ek[m] + k, newEk + k) == 0)
								{
									if(nbEltAlreadySeen >= 1)
									{//If two edge cliques have 2 elements in common then it is the same edge clique.
										addEk = false;
										goto endEkExists;
									}
									nbEltAlreadySeen++;
								}
							}
						}
						endEkExists:
						if(addEk)
						{
							ekCert.ek[ekCert.nbEk] = (NUPLE*)malloc(p * sizeof(NUPLE));
							if(!ekCert.ek[ekCert.nbEk])
								NO_MEM_LEFT()
							for(k = 0 ; k < p ; k++)
							{
								ekCert.ek[ekCert.nbEk][k].tab = (unsigned int*)malloc(g->nbVertices * sizeof(unsigned int));
								if(!ekCert.ek[ekCert.nbEk][k].tab)
									NO_MEM_LEFT()
								nupleCpy(newEk + k, ekCert.ek[ekCert.nbEk] + k);
								listVerticesPGOrderedBool[positionNuple[k]] = true;
							}
							ekCert.nbEk++;
						}
					}
				}
			}
		}
	}

	//We free some useless memory.
	//We reallocate ekCert.ek to the right size.
	//------------ It is not clear this realloc is such a good idea...
	ekCert.ek = (NUPLE**)realloc(ekCert.ek, ekCert.nbEk * sizeof(NUPLE**));

	free(positionNuple);
	for(k = 0 ; k < p ; k++)
		free(newEk[k].tab);
	free(newEk);
	free(listVerticesPGOrderedBool);
	free(newEkBool);

	if(field == 2)
	{//If the field is F2
		MATRIX_F2 ekVSVertices;
		//Since I prefer doing Gauss pivoting on the lines rather than on the columns, I choose to consider this matrix and not its transpose.
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

//--------- Start Gauss
		MATRIX_F2 q;
		unsigned long max, rank = 0, firstNonZeroEntry = 0;
		char *tmp = NULL;

		q.nbRows = ekCert.nbEk;
		q.nbColumns = ekCert.nbEk;

		q.mat = (char**)malloc(q.nbRows * sizeof(char*));
		if(!q.mat)
			NO_MEM_LEFT()
		for(i = 0 ; i < q.nbRows ; i++)
		{
			q.mat[i] = (char*)calloc(q.nbColumns, q.nbColumns * sizeof(char));
			if(!q.mat[i])
				NO_MEM_LEFT()
			q.mat[i][i] = 1;
		}

		//We start Gauss pivoting
		for(i = 0 ; i < ekVSVertices.nbRows ; i++)
		{
			//We look for the row corresponding to the maximum binary integer.
			max = i;
			for(j = max + 1 ; j < ekVSVertices.nbRows ; j++)
			{
				for(k = 0 ; k < ekVSVertices.nbColumns ; k++)
				{
					if(ekVSVertices.mat[j][k] < ekVSVertices.mat[max][k])
						break;
					else if(ekVSVertices.mat[j][k] > ekVSVertices.mat[max][k])
					{
						max = j;
						break;
					}
				}
			}
			//We test wether the maximum line is zero and retreive the first non zero entry.
			for(j = 0 ; j < ekVSVertices.nbColumns ; j++)
			{
				if(ekVSVertices.mat[max][j] != 0)
				{
					firstNonZeroEntry = j;
					break;
				}
			}
			if(j == ekVSVertices.nbColumns)
			//If the maximum line is zero then we have the rank.
				break;
			else if(max > i)
			{//We put the max in position i.
				tmp = ekVSVertices.mat[i];
				ekVSVertices.mat[i] = ekVSVertices.mat[max];
				ekVSVertices.mat[max] = tmp;

				//We perform the same operation on matrix q.
				tmp = q.mat[i];
				q.mat[i] = q.mat[max];
				q.mat[max] = tmp;
			}
			rank++;
			if(rank == ekVSVertices.nbRows || rank == ekVSVertices.nbColumns)
				break;
			//We xor every line under and above line i that needs to be xored.
			for(j = i + 1 ; j < ekVSVertices.nbRows ; j++)
			{
				if(ekVSVertices.mat[j][firstNonZeroEntry] != 0)
				{//We have found a non zero coef so we must xor line j !
					for(k = 0 ; k < ekVSVertices.nbColumns ; k++)
						ekVSVertices.mat[j][k] = (ekVSVertices.mat[j][k] + ekVSVertices.mat[i][k]) % 2;
					//We perform the same operation for matrix q.
					for(k = 0 ; k < q.nbColumns ; k++)
						q.mat[j][k] = (q.mat[j][k] + q.mat[i][k]) % 2;
				}
			}
		}
//--------- End Gauss

		unsigned int nb1OnThisRow, nb1OnQRow, nb1OnQRowOld = q.nbColumns + 1, bestRow = q.nbRows + 1;
		for(i = 0 ; i < ekVSVertices.nbRows ; i++)
		{
			nb1OnThisRow = 0;
			nb1OnQRow = 0;
			for(j = 0 ; j < ekVSVertices.nbColumns ; j++)
			{
				if(ekVSVertices.mat[i][j] == 1)
					nb1OnThisRow++;
			}
			if(nb1OnThisRow == 1)
			{
				//We count the number of 1 in the corresponding q row (We want to minimize it !).
				for(j = 0 ; j < q.nbColumns ; j++)
				{
					if(q.mat[i][j] == 1)
						nb1OnQRow++;
					if(nb1OnQRow > nb1OnQRowOld)
						break;
				}
				if(nb1OnQRowOld > nb1OnQRow)
				{
					nb1OnQRowOld = nb1OnQRow;
					bestRow = i;
				}
			}
		}
		if(bestRow <= q.nbRows)
		{//If we found some row with exactly one 1 in the matrix in row echelon form.
			ekCert.weight = (long double*)calloc(ekCert.nbEk, ekCert.nbEk * sizeof(long double));
			if(!ekCert.weight)
				NO_MEM_LEFT()
			for(j = 0 ; j < q.nbColumns ; j++)
				ekCert.weight[j] = q.mat[bestRow][j];
		}
		else
			ekCert.weight = NULL;

		//We free useless memory.
		for(i = 0 ; i < ekCert.nbEk ; i++)
			free(ekVSVertices.mat[i]);
		free(ekVSVertices.mat);
		for(i = 0 ; i < q.nbRows ; i++)
			free(q.mat[i]);
		free(q.mat);
	}

	free(listVerticesPGOrdered);
	return ekCert;
}

void freeEkCert(EK_CERT *ekCert)
{
	unsigned long i,j;
	for(i = 0 ; i < ekCert->nbEk ; i++)
	{
		for(j = 0 ; j < ekCert->nbEltPerEk ; j++)
			free(ekCert->ek[i][j].tab);
		free(ekCert->ek[i]);
	}
	free(ekCert->ek);
	if(ekCert->weight)
		free(ekCert->weight);
}
