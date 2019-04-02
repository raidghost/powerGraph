#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "structs.h"
#include "tools.h"

GRAPH_LIST* addVertex(GRAPH_LIST *g, NUPLE *v)
{//Add a vertex in a graphList if it does not exist. If g is empty then a GRAPH_LIST is created.
	unsigned long i;
	if(g == NULL)
	{
		GRAPH_LIST* newVertex = (GRAPH_LIST*)malloc(sizeof(GRAPH_LIST));
		if(newVertex == NULL)
			NO_MEM_LEFT()

		newVertex->next = NULL;

		newVertex->v.length = v->length;
		newVertex->v.tab = (unsigned int*)malloc(newVertex->v.length * sizeof(unsigned int));
		if(newVertex->v.tab == NULL)
			NO_MEM_LEFT()
		for(i = 0 ; i < v->length ; i++)
			newVertex->v.tab[i] = v->tab[i];
		
		newVertex->nbNeighbours = 0;
		newVertex->neighbours = NULL;
		return newVertex;
	}
	int comparison = nupleCmp(v, &(g->v));
	if(comparison < 0)
	{
		GRAPH_LIST* newVertex = (GRAPH_LIST*)malloc(sizeof(GRAPH_LIST));
		if(newVertex == NULL)
			NO_MEM_LEFT()
		
		newVertex->next = g;

		newVertex->v.length = v->length;
		newVertex->v.tab = (unsigned int*)malloc(newVertex->v.length * sizeof(unsigned int));
		if(newVertex->v.tab == NULL)
			NO_MEM_LEFT()
		for(i = 0 ; i < v->length ; i++)
			newVertex->v.tab[i] = v->tab[i];

		newVertex->nbNeighbours = 0;
		newVertex->neighbours = NULL;
		return newVertex;
	}
	else if(comparison == 0)
		return g;

	GRAPH_LIST *tmp = g->next, *tmpOld = g;
	while(tmp != NULL)
	{
		comparison = nupleCmp(v, &(tmp->v));
		if(comparison < 0)
		{
			GRAPH_LIST* newVertex = (GRAPH_LIST*)malloc(sizeof(GRAPH_LIST));
			if(newVertex == NULL)
				NO_MEM_LEFT()

			tmpOld->next = newVertex;
			newVertex->next = tmp;

			newVertex->v.length = v->length;
			newVertex->v.tab = (unsigned int*)malloc(newVertex->v.length * sizeof(NUPLE));
			if(newVertex->v.tab == NULL)
				NO_MEM_LEFT()
			for(i = 0 ; i < v->length ; i++)
				newVertex->v.tab[i] = v->tab[i];
					
			newVertex->nbNeighbours = 0;
			newVertex->neighbours = NULL;
			return g;
		}
		else if(comparison == 0)
			return g;
		tmpOld = tmp;
		tmp = tmp->next;
	}
	GRAPH_LIST* newVertex = (GRAPH_LIST*)malloc(sizeof(GRAPH_LIST));
	if(newVertex == NULL)
		NO_MEM_LEFT()

	tmpOld->next = newVertex;
	newVertex->next = NULL;

	newVertex->v.length = v->length;
	newVertex->v.tab = (unsigned int*)malloc(newVertex->v.length * sizeof(unsigned int));
	if(newVertex->v.tab == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < v->length ; i++)
		newVertex->v.tab[i] = v->tab[i];

	newVertex->nbNeighbours = 0;
	newVertex->neighbours = NULL;
	return g;
}

GRAPH_LIST* searchVertex(GRAPH_LIST *g, NUPLE v)
{//Return the adress of vertex v in g or NULL if not exists.
	GRAPH_LIST *tmp = g, *tmpOld = g;
	int comparison = nupleCmp(&v, &(g->v));
	if(comparison < 0)
		return NULL;
	else if(comparison == 0)
		return g;
	while(tmp != NULL && comparison > 0)
	{
		comparison = nupleCmp(&v, &(tmp->v));
		tmpOld = tmp;
		tmp = tmp->next;
	}
	if(comparison == 0)
		return tmpOld;
	return NULL;
}

void addEdge(GRAPH_LIST* g, NUPLE u, NUPLE v)
{
	GRAPH_LIST *positionU = NULL, *positionV = NULL;
	positionU = searchVertex(g, u);
	if(positionU == NULL)
		return;
	positionV = searchVertex(g, v);
	if(positionV == NULL)
		return;
	positionU->neighbours = (GRAPH_LIST**)realloc(positionU->neighbours, (positionU->nbNeighbours + 1) * sizeof(GRAPH_LIST**));
	if(positionU->neighbours == NULL)
		NO_MEM_LEFT()
	positionU->neighbours[positionU->nbNeighbours] = positionV;
	positionU->nbNeighbours++;

	positionV->neighbours = (GRAPH_LIST**)realloc(positionV->neighbours, (positionV->nbNeighbours + 1)*sizeof(GRAPH_LIST*));
	if(positionV->neighbours == NULL)
		NO_MEM_LEFT()
	positionV->neighbours[positionV->nbNeighbours] = positionU;
	positionV->nbNeighbours++;
}

void freeGraphList(GRAPH_LIST *g)
{
	GRAPH_LIST *tmp = g, *tmpOld = g;
	while(tmp != NULL)
	{
		free(tmp->v.tab);
		if(tmp->neighbours != NULL)
			free(tmp->neighbours);
		tmpOld = tmp;
		tmp = tmp->next;
		free(tmpOld);
	}
}
