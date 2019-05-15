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
	int i;
	bool makeExp = false, ekCert = false, checkNbVertices = false, checkNbEdges = false, checkCommonDegree = false;
	unsigned int nbVerticesExpected = 0, nbEdgesExpected = 0;
	int degreeMax = -1, supportMax = -1, verbose = 0, field = 2, p = 2, commonDegreeExpected = 0;
	char *tmp;
	GRAPH g;
	GRAPH graphExp;
	GRAPH_LIST* graphExpList = NULL;

	if(argc < 3)
	{
		fprintf(stderr, "Usage : prog [--makeExp=?] [--supportMax=?] [--ekCert] [--nbVertices=?] [--nbEdges=?] [--commonDegree=?] GRAPH_FILE field\nPossible values for field are : F2, R\n");
		return EXIT_FAILURE;
	}
	for(i = 1 ; i < argc - 2; i++)
	{
		if(strncmp(argv[i], "--makeExp", 9) == 0)
		{
			makeExp = true;
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp && string2Int(tmp) >= 1)
				p = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of p must be >= 1. We set it to 3 by default.\n");
				p = 3;
			}
		}
		else if(strncmp(argv[i], "--supportMax", 12) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
				supportMax = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of supportMax can be any non negative integer. If it is strictly negative, then we won't bound the support. We set it to -1 by default.\n");
				supportMax = -1;
			}
		}
		else if(strncmp(argv[i], "--degreeMax", 11) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
				degreeMax = string2Int(tmp);
			else
			{
				fprintf(stderr, "The value of degreeMax can be any non negative integer. If it is strictly negative, then we won't bound the degree. We set it to -1 by default.\n");
				degreeMax = -1;
			}
		}
		else if(strcmp(argv[i], "--ekCert") == 0)
			ekCert = true;
		else if(strncmp(argv[i], "--nbVertices", 12) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
			{
				nbVerticesExpected = string2Int(tmp);
				checkNbVertices = true;
			}
			else
				fprintf(stderr, "The number of vertices must be a non negative integer.\n");
		}
		else if(strncmp(argv[i], "--nbEdges", 9) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
			{
				nbEdgesExpected = string2Int(tmp);
				checkNbEdges = true;
			}
			else
				fprintf(stderr, "The number of edges must be a non negative integer.\n");
		}
		else if(strncmp(argv[i], "--commonDegree", 14) == 0)
		{
			strtok(argv[i], "=");
			tmp = strtok(NULL, "=");
			if(tmp)
			{
				commonDegreeExpected = string2Int(tmp);
				checkCommonDegree = true;
			}
			else
				fprintf(stderr, "The common degree must be a non negative integer.\n");
		}
		else if(strncmp(argv[i], "-v", 2) == 0)
		{
			if(strcmp(argv[i], "-v") == 0)
				verbose = 1;
			else if(strcmp(argv[i], "-vv") == 0)
				verbose = 2;
			else
				verbose = 3;
		}
		else
			fprintf(stderr, "Sorry, I don't know the argument \"%s\".\n", argv[i]);
	}

	g = loadGraphFromFile(argv[argc-2]);
	if(g.mat == NULL)
	{//We cannot open the file containning the graph.
		fprintf(stderr, "Unable to read the graph from \"%s\".\n", argv[argc - 2]);
		return EXIT_FAILURE;
	}

	if(strcmp(argv[argc-1], "R") == 0)
		field = 0;
	else if(strcmp(argv[argc-1], "F2") == 0)
		field = 2;
	else
	{
		fprintf(stderr, "The field should be either \"F2\" or \"R\". We use F2 by default.\n");
		field = 2;
	}

	if(makeExp)
	{
		graphExpList = genPowerGraph(&g, p, supportMax, degreeMax);
		if(verbose >= 3)
			displayGraphList(graphExpList);
		graphExp = graphList2Mat(graphExpList);
	}

	if(ekCert)
	{
		if(!makeExp)
			fprintf(stderr, "We can only look for an edge clique certificate in a power graph. Use \"--makeExp\".\n");
		else
		{
			if(field == 2)
			{
				EK_CERT_F2 ekCertificate;
				ekCertificate = findEkCertF2(&g, graphExpList, p);
				if(ekCertificate.weight)
				{//If we have found an edge clique certificate.
					printf("We found an edge clique certificate !\n");
					if(verbose >= 1)
						displayEkCertF2(&ekCertificate, false);
					else
						printf("Use -v to display it.\n");
					printf("We will now check if this certificate is valid.\n");
					GRAPH_LIST* center = checkEkCertF2(graphExpList, &ekCertificate);
					if(center)
					{
						printf("This certificate is \e[1mvalid\e[0m ! Its center is : ");
						displayNuple(&center->v);
						printf("\n");
					}
					else
						printf("Unfortunately, this certificate is \e[1minvalid\e[0m.\n");
				}
				else
				{
					printf("We could not find any edge clique certificate.\n");
					if(supportMax >= 0)
						printf("Perhaps you should increase the support (\"--supportMax\").\n");
				}
				freeEkCertF2(&ekCertificate);
			}
			else
			{
				EK_CERT_R ekCertificate;
				ekCertificate = findEkCertR(&g, graphExpList, p, verbose);
				if(ekCertificate.weight)
				{//If we have found an edge clique certificate.
					printf("We found an edge clique certificate !\n");
					if(verbose >= 1)
						displayEkCertR(&ekCertificate, false);
					else
						printf("Use -v to display it.\n");
					printf("We will now check if this certificate is valid.\n");
					GRAPH_LIST* center = checkEkCertR(graphExpList, &ekCertificate);
					if(center)
					{
						printf("This certificate is \e[1mvalid\e[0m ! Its center is : ");
						displayNuple(&center->v);
						printf("\n");
					}
					else
						printf("Unfortunately, this certificate is \e[1minvalid\e[0m.\n");
				}
				else
				{
					printf("We could not find any edge clique certificate.\n");
					if(supportMax >= 0)
						printf("Perhaps you should increase the support (\"--supportMax\").\n");
				}
				freeEkCertR(&ekCertificate);
			}
		}
	}

	if(checkNbVertices)
	{
		if(makeExp)
		{//If we have built the power graph, then we make the tests on it.
			if(graphExp.nbVertices != nbVerticesExpected)
				printf("\e[1mError\e[0m : The graph %d^G has %ld vertices instead of %d\n", p, graphExp.nbVertices, nbVerticesExpected);
			else
				printf("The number of vertices of %d^G is \e[1mOK\e[0m (%ld).\n", p, graphExp.nbVertices);
		}
		else
		{
			if(g.nbVertices != nbVerticesExpected)
				printf("\e[1mError\e[0m : The graph G has %ld vertices instead of %d\n", g.nbVertices, nbVerticesExpected);
			else
				printf("The number of vertices of G is \e[1mOK\e[0m (%ld).\n", g.nbVertices);
		}
	}

	if(checkNbEdges)
	{
		if(makeExp)
		{//If we have built the power graph, then we make the tests on it.
			unsigned int nbEdgesG = nbEdges(&graphExp);
			if(nbEdgesExpected != nbEdgesG)
				printf("\e[1mError\e[0m : The graph %d^G has %d edges instead of %d\n", p, nbEdgesG, nbEdgesExpected);
			else
				printf("The number of edges of %d^G is \e[1mOK\e[0m (%d).\n", p, nbEdgesG);
		}
		else
		{
			unsigned int nbEdgesG = nbEdges(&g);
			if(nbEdgesExpected != nbEdgesG)
				printf("\e[1mError\e[0m : The graph G has %d edges instead of %d\n", nbEdgesG, nbEdgesExpected);
			else
				printf("The number of edges of G is \e[1mOK\e[0m (%d).\n", nbEdgesG);
		}
	}

	if(checkCommonDegree)
	{
		if(makeExp)
		{//If we have built the power graph, then we make the tests on it.
			int commonDegreeG = commonDegree(&graphExp);
			if(commonDegreeG == -1)
				printf("The graph %d^G has no common degree.\n", p);
			else if(commonDegreeG != commonDegreeExpected)
				printf("\e[1mError\e[0m : The graph %d^G has common degree %d instead of %d.\n", p, commonDegreeG, commonDegreeExpected);
			else
				printf("The common degree of %d^G is \e[1mOK\e[0m (%d).\n", p, commonDegreeG);
		}
		else
		{
			int commonDegreeG = commonDegree(&g);
			if(commonDegreeG == -1)
				printf("The graph G has no common degree.\n");
			else if(commonDegreeG != commonDegreeExpected)
				printf("\e[1mError\e[0m : The graph G has common degree %d instead of %d.\n", commonDegreeG, commonDegreeExpected);
			else
				printf("The common degree of G is \e[1mOK\e[0m (%d).\n", commonDegreeG);
		}
	}

	if(makeExp)
	{
		freeGraph(&graphExp);
		freeGraphList(graphExpList);
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

GRAPH_LIST* checkEkCertF2(GRAPH_LIST* powerGraph, EK_CERT_F2* ekCert)
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
	char* listVerticesOrderedWeight;
	GRAPH_LIST* tmp;

	listVerticesOrdered = (NUPLE*)malloc(nbVerticesPG * sizeof(NUPLE));
	listVerticesOrderedWeight = (char*)calloc(nbVerticesPG, nbVerticesPG * sizeof(char));

	if(!listVerticesOrdered || !listVerticesOrderedWeight)
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
				listVerticesOrderedWeight[position] = (listVerticesOrderedWeight[position] + ekCert->weight[i]) % 2;
		}
	}
	unsigned int nbZeroEntries = 0;
	for(i = 0 ; i < nbVerticesPG ; i++)
	{
		if((int)listVerticesOrderedWeight[i] % 2)
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
	if(nbZeroEntries != 1)
		center = NULL;
	free(listVerticesOrdered);
	free(listVerticesOrderedWeight);
	return center;
}

GRAPH_LIST* checkEkCertR(GRAPH_LIST* powerGraph, EK_CERT_R* ekCert)
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
	mpq_t* listVerticesOrderedWeight;
	mpq_t zero;
	GRAPH_LIST* tmp;

	mpq_init(zero);

	listVerticesOrdered = (NUPLE*)malloc(nbVerticesPG * sizeof(NUPLE));
	listVerticesOrderedWeight = (mpq_t*)malloc(nbVerticesPG * sizeof(mpq_t));

	if(!listVerticesOrdered || !listVerticesOrderedWeight)
		NO_MEM_LEFT()
	for(i = 0 ; i < nbVerticesPG ; i++)
		mpq_init(listVerticesOrderedWeight[i]);
	
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
				mpq_clear(zero);
				for(i = 0 ; i < nbVerticesPG ; i++)
					mpq_clear(listVerticesOrderedWeight[i]);
				free(listVerticesOrderedWeight);
				return NULL;
			}
			else
				mpq_add(listVerticesOrderedWeight[position], listVerticesOrderedWeight[position], ekCert->weight[i]);
		}
	}
	unsigned int nbZeroEntries = 0;
	for(i = 0 ; i < nbVerticesPG ; i++)
	{
		if(mpq_cmp(listVerticesOrderedWeight[i],zero))
		{
			nbZeroEntries++;
			if(nbZeroEntries > 1)
			{
				free(listVerticesOrdered);
				mpq_clear(zero);
				for(i = 0 ; i < nbVerticesPG ; i++)
					mpq_clear(listVerticesOrderedWeight[i]);
				free(listVerticesOrderedWeight);
				return NULL;
			}
			center = searchVertex(powerGraph, listVerticesOrdered[i]);
		}
	}
	if(nbZeroEntries != 1)
		center = NULL;
	free(listVerticesOrdered);
	mpq_clear(zero);
	for(i = 0 ; i < nbVerticesPG ; i++)
		mpq_clear(listVerticesOrderedWeight[i]);
	free(listVerticesOrderedWeight);
	return center;
}
