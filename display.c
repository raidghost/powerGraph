#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

void displayGraph(const GRAPH* g)
{
	unsigned long i,j;
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		for(j = 0 ; j < g->nbVertices ; j++)
			printf("%d ", (int)g->mat[i][j]);
		printf("\n");
	}
}

void displayDn(const DN* dn)
{
	unsigned long i,j;
	for(i = 0 ; i < dn->nbTuples ; i++)
	{
		for(j = 0 ; j < dn->n ; j++)
		{
			if(j == 0)
				printf("%d", dn->tuples[i][j]);
			else
				printf("-%d", dn->tuples[i][j]);
		}
		printf("\n");
	}
}

void displayMatrixF2(const MATRIX_F2* m)
{
	unsigned long i,j;
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j < m->nbColumns ; j++)
			printf("%d ", (int)m->mat[i][j]);
		printf("\n");
	}
}

void displayMatrixR(const MATRIX_R* m)
{
	unsigned long i,j;
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j < m->nbColumns ; j++)
		{
			if(m->mat[i][j] < 0)
				printf("%Lf ", m->mat[i][j]);
			else
				printf(" %Lf ", m->mat[i][j]);
		}
		printf("\n");
	}
}
