#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "io.h"

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

void displayMatrix(const MATRIX* m)
{
	unsigned long i,j;
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j < m->nbColumns ; j++)
			printf("%d ", (int)m->mat[i][j]);
		printf("\n");
	}
}

void printMatrixMpz(const MATRIX_MPZ* m)
{
	char* chain;
	unsigned long i,j;

	chain = (char*)malloc((m->nbColumns + 1) * sizeof(char));
	if(chain == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j <= m->nbColumns ; j++)
			chain[j] = '\0';
		mpz_get_str(chain, 2, m->mat[i]);
		printf("%s\n", chain);
	}
	free(chain);
}
