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

unsigned long getNbVertices(GRAPH_LIST* g)
{
	unsigned long nbVertices = 0;
	GRAPH_LIST* tmp = g;
	while(tmp != NULL)
	{
		tmp = tmp->next;
		nbVertices++;
	}
	return nbVertices;
}

GRAPH_LIST* searchVertex(GRAPH_LIST* g, NUPLE v)
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

GRAPH_LIST* getVertexByIndex(GRAPH_LIST* g, unsigned int index)
{//This function returns the vertex in position "index" in the list (starting at 0).
	unsigned int i;
	GRAPH_LIST *tmp = g;
	for(i = 0 ; i < index && tmp != NULL ; i++)
		tmp = tmp->next;
	if(i == index)
		return tmp;
	else
		return NULL;
}

int getIndexByVertex(GRAPH_LIST* g, GRAPH_LIST* u)
{//Given a vertex u returns the position of u in g (-1 if not exists).
	int position = 0;
	GRAPH_LIST* tmp = g;
	while(tmp != NULL)
	{
		if(u == tmp)
			return position;
		position++;
		tmp = tmp->next;
	}
	return -1;
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

void addEdgeFast(GRAPH_LIST* u, GRAPH_LIST* v)
{//Assuming u and v to be valid pointer to vertices of g. We can easily add the edges.
	bool addUV = true, addVU = true;
	unsigned int i;
	for(i = 0 ; i < u->nbNeighbours ; i++)
	{
		if(u->neighbours[i] == v)
		{
			addUV = false;
			break;
		}
	}
	for(i = 0 ; i < v->nbNeighbours ; i++)
	{
		if(v->neighbours[i] == u)
		{
			addVU = false;
			break;
		}
	}
	if(addUV)
	{
		u->neighbours = (GRAPH_LIST**)realloc(u->neighbours, (u->nbNeighbours + 1) * sizeof(GRAPH_LIST*));
		if(u->neighbours == NULL)
			NO_MEM_LEFT()
		u->neighbours[u->nbNeighbours] = v;
		u->nbNeighbours++;
	}
	if(addVU)
	{
		v->neighbours = (GRAPH_LIST**)realloc(v->neighbours, (v->nbNeighbours + 1) * sizeof(GRAPH_LIST*));
		if(v->neighbours == NULL)
			NO_MEM_LEFT()
		v->neighbours[v->nbNeighbours] = u;
		v->nbNeighbours++;
	}
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

GRAPH graphList2Mat(GRAPH_LIST* graphList)
{//Returns a graph of type GRAPH (which is basically a matrix)
	unsigned long i,j;
	int index;
	GRAPH graph;
	GRAPH_LIST* tmp = graphList;
	graph.nbVertices = getNbVertices(graphList);
	graph.mat = (char**)malloc(graph.nbVertices * sizeof(char*));
	if(graph.mat == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < graph.nbVertices ; i++)
	{
		graph.mat[i] = (char*)calloc(graph.nbVertices, graph.nbVertices * sizeof(char));
		if(graph.mat[i] == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < tmp->nbNeighbours ; j++)
		{
			index = getIndexByVertex(graphList, tmp->neighbours[j]);
			if(index == -1)
			{
				fprintf(stderr, "Some vertex cannot be found in the graph. It means there is a problem somewhere in the library.\n");
				exit(EXIT_FAILURE);
			}
			graph.mat[i][index] = 1;
		}
		tmp = tmp->next;
	}
	return graph;
}
