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

/*void displayMatrixR(const MATRIX_R* m)
{
	unsigned long i,j;
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j < m->nbColumns ; j++)
		{
			if(mpq_cmp_ui(m->mat[i][j],0,1) < 0)
				gmp_printf("%Qd ", m->mat[i][j]);
			else
				gmp_printf(" %Qd ", m->mat[i][j]);
		}
		printf("\n");
	}
}*/

void displayMatrixR(const MATRIX_R* m)
{
	FILE *f = fopen("matrix", "w");
	unsigned long i,j;

	fprintf(f, "%ld %ld\n", m->nbRows, m->nbColumns);
	for(i = 0 ; i < m->nbRows ; i++)
	{
		for(j = 0 ; j < m->nbColumns ; j++)
		{
			if(mpq_cmp_ui(m->mat[i][j],0,1) < 0)
			{
				//gmp_printf("%Qd ", m->mat[i][j]);
				fputs("-1 ", f);
			}
			else if(mpq_cmp_ui(m->mat[i][j],0,1) > 0)
			{
				//gmp_printf(" %Qd ", m->mat[i][j]);
				fputs("1 ", f);
			}
			else
			{
				//printf("0 ");
				fputs("0 ", f);
			}
		}
		fputc('\n', f);
		//printf("\n");
	}
	fclose(f);
}
