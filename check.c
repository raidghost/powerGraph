/*This program aims at testing the main program.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "display.h"
#include "io.h"
#include "graphList.h"
#include "powerGraph.h"
#include "structs.h"
#include "tools.h"

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog GRAPH_FILE [nbVertices [nbEdges] [commonDegree]] [--testEkCert supportMax p field]\n");
		return EXIT_FAILURE;
	}
	GRAPH g = loadGraphFromFile(argv[1]);
	if(strcmp(argv[2], "--testEkCert") != 0)
	{
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
	}
	else if(strcmp(argv[2], "--testEkCert") == 0 && argc > 5)
	{//In that case, we generate the powerGraph of the graph given in input.
		int field, p, supportMax;
		supportMax = string2Int(argv[3]);
		if(supportMax < 0)
			supportMax = g.nbVertices;
		p = string2Int(argv[4]);
		if(p < 1)
		{
			fprintf(stderr, "The number p must be an integer bigger than 1.\n");
			exit(EXIT_FAILURE);
		}

		if(strcmp(argv[4], "F2") == 0)
			field = 2;
		else if(strcmp(argv[4], "R") == 0)
			field = -1;
		else
			field = 2;

		GRAPH graphExp;
		GRAPH_LIST* graphExpList;
		EK_CERT ekCertificate;
		graphExpList = genPowerGraph(&g, p, supportMax);
//		if(verbose >= 3)
//			displayGraphList(graphExpList);
		graphExp = graphList2Mat(graphExpList);
		ekCertificate = findEkCert(&g, graphExpList, p, field);
		if(ekCertificate.weight)
		{//If we have found an edge clique certificate.
			printf("We found an edge clique certificate !\n");
			displayEkCert(&ekCertificate, false);
			GRAPH_LIST* checkCertif = checkEkCert(graphExpList, &ekCertificate, field);
			if(checkCertif)
			{
				printf("This certificate is valid ! The center is : ");
				displayNuple(&checkCertif->v);
			}
			else
				printf("Unfortunately, this certificate is not valid...\n");
		}
		else
			printf("We could not find any certificate...\n");
		freeEkCert(&ekCertificate);
		freeGraphList(graphExpList);
		freeGraph(&graphExp);

	}
	freeGraph(&g);
	return EXIT_SUCCESS;
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

GRAPH_LIST* checkEkCert(GRAPH_LIST* powerGraph, EK_CERT* ekCert, int field)
{//Test wether an edge clique certificate is valid for the given powerGraph. If yes returns a pointer to the center and if not returns NULL.
	if(!ekCert->weight)
	{
		fprintf(stderr, "The weight are not set for this edge clique certificate.\n");
		return NULL;
	}
	unsigned long i,j;
	int position;
	unsigned int nbVerticesPG = getNbVertices(powerGraph);
	if(nbVerticesPG == 0)
	{
		fprintf(stderr, "The powerGraph has no vertices.\n");
		return NULL;
	}
	NUPLE* listVerticesOrdered;
	GRAPH_LIST* center = NULL;
	long double* listVerticesOrderedWeight;
	GRAPH_LIST* tmp;

	listVerticesOrdered = (NUPLE*)malloc(nbVerticesPG * sizeof(NUPLE));
	listVerticesOrderedWeight = (long double*)calloc(nbVerticesPG, nbVerticesPG * sizeof(long double));

	if(!listVerticesOrdered || ! listVerticesOrderedWeight)
		NO_MEM_LEFT()
	
	tmp = powerGraph;
	for(i = 0 ; i < nbVerticesPG ; i++)
	{
		listVerticesOrdered[i] = tmp->v;
		tmp = tmp->next;
	}

	for(i = 0 ; i < ekCert->nbEk ; i++)
	{
		for(j = 0 ; j < ekCert->nbEltPerEk ; j++)
		{
			position = dichoSearchNupleList(listVerticesOrdered, ekCert->ek[i] + j, 0, nbVerticesPG - 1);
			if(position == -1)
			{
				fprintf(stderr, "The following vertex does not exists in the powerGraph and it should !\n");
				displayNuple(ekCert->ek[i] + j);
				free(listVerticesOrdered);
				free(listVerticesOrderedWeight);
				return NULL;
			}
			else
				listVerticesOrderedWeight[position] += ekCert->weight[i];
		}
	}
	unsigned int nbZeroEntries = 0;
	if(field == -1)
	{
		for(i = 0 ; i < nbVerticesPG ; i++)
		{
			if(listVerticesOrderedWeight[i])
			{
				nbZeroEntries++;
				if(nbZeroEntries > 1)
				{
					free(listVerticesOrdered);
					free(listVerticesOrderedWeight);
					return NULL;
				}
				center = searchVertex(powerGraph, listVerticesOrdered[i]);
			}
		}
	}
	else
	{
		for(i = 0 ; i < nbVerticesPG ; i++)
		{
			if((int)listVerticesOrderedWeight[i] % field)
			{
				nbZeroEntries++;
				if(nbZeroEntries > 1)
				{
					free(listVerticesOrdered);
					free(listVerticesOrderedWeight);
					return NULL;
				}
				center = searchVertex(powerGraph, listVerticesOrdered[i]);
			}
		}
	}
	if(nbZeroEntries != 1)
		center = NULL;
	free(listVerticesOrdered);
	free(listVerticesOrderedWeight);
	return center;
}
