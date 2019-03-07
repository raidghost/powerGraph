#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "limits.h"
#include "structs.h"
#include "io.h"

int string2Int(const char *string)
{//Convert a string to an integer in basis 10.
	int result = 0, sign = 1;
	unsigned int stringLength = strlen(string), i = 0, power10 = 0;

	if(string[0] == '-')
	{
		sign = -1;
		i++;
	}
	power10 = pow(10,stringLength-1-i);
	while(i < stringLength)
	{
		result += power10 * (int)(string[i] - '0');
		power10 /= 10;
		i++;
	}
	return result * sign;
}

GRAPH loadGraphFromFile(char *fileName)
{
	char nbVerticesChar[NB_CHAR_MAX] = {0}, line[NB_CHAR_MAX] = {0}, vertex1Char[NB_CHAR_MAX] = {0}, vertex2Char[NB_CHAR_MAX] = {0};
	GRAPH g;
	g.mat = NULL;
	g.nbVertices = 0;
	int vertex1 = 0, vertex2 = 0;
	unsigned long i = 0, j = 0;
	FILE *file = NULL;
	file = fopen(fileName, "r");
	if(file == NULL)
		return g;
	if(!fscanf(file, "%s\n", nbVerticesChar))
		return g;
	g.nbVertices = string2Int(nbVerticesChar);

	g.mat = (char**)malloc(g.nbVertices * sizeof(char*));
	if(g.mat == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < g.nbVertices ; i++)
	{
		g.mat[i] = (char*)malloc(g.nbVertices * sizeof(char));
		if(g.mat[i] == NULL)
			NO_MEM_LEFT()
	}

	while(fscanf(file, "%s\n", line) != EOF)
	{
		for(i = 0 ; i < NB_CHAR_MAX ; i++)
		{
			if(line[i] != '-')
				vertex1Char[i] = line[i];
			else
			{
				vertex1Char[i] = '\0';
				j = i;
				break;
			}
		}
		while(i < NB_CHAR_MAX && line[i+1] != '\n')
		{
			vertex2Char[i-j] = line[i+1];
			i++;
		}
		vertex2Char[i-j] = '\0';
		vertex1 = string2Int(vertex1Char);
		vertex2 = string2Int(vertex2Char);
		g.mat[vertex1][vertex2] = 1;
		g.mat[vertex2][vertex1] = 1;
	}
	return g;
}

void freeGraph(GRAPH* g)
{
	unsigned long i;
	for(i = 0 ; i < g->nbVertices ; i++)
		free(g->mat[i]);
	free(g->mat);
}
