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
			if(mpq_cmp_ui(m->mat[i][j],0,1) < 0)
				gmp_printf("%Qd ", m->mat[i][j]);
			else
				gmp_printf(" %Qd ", m->mat[i][j]);
		}
		printf("\n");
	}
}

void displayNuple(NUPLE* n)
{
	unsigned int i;
	for(i = 0 ; i < n->length - 1 ; i++)
	{
		if(i < n->length - 1)
			printf("%d-", n->tab[i]);
	}
	printf("%d", n->tab[i]);
}

void displayGraphList(GRAPH_LIST* g)
{
	GRAPH_LIST *tmp = g;
	unsigned int i,j;
	while(tmp != NULL)
	{
		for(i = 0 ; i < tmp->v.length ; i++)
		{
			if(i < tmp->v.length - 1)
				printf("%d-", tmp->v.tab[i]);
			else
				printf("%d -> \n", tmp->v.tab[i]);
		}
		for(i = 0 ; i < tmp->nbNeighbours ; i++)
		{
			printf("\t");
			for(j = 0 ; j < tmp->neighbours[i]->v.length ; j++)
			{
				if(j < tmp->neighbours[i]->v.length - 1)
					printf("%d-", tmp->neighbours[i]->v.tab[j]);
				else
					printf("%d\n", tmp->neighbours[i]->v.tab[j]);
			}
		}
		tmp = tmp->next;
	}
}

void displayEkCertF2(EK_CERT_F2* ekCert, bool displayZeroWeight)
{
	unsigned int i,j;
	unsigned int nbEkUsedInCert = 0;
	for(i = 0 ; i < ekCert->nbEk ; i++)
	{
		for(j = 0 ; j < ekCert->nbEltPerEk ; j++)
		{
			if(displayZeroWeight || ekCert->weight[i])
			{
				displayNuple(ekCert->ek[i] + j);
				if(j < ekCert->nbEltPerEk - 1)
					printf("---");
			}
		}
		if(displayZeroWeight || ekCert->weight[i])
			printf("\tweight = %d\n", (int)ekCert->weight[i]);
		if(ekCert->weight[i])
			nbEkUsedInCert++;
	}
	printf("There are %d edge cliques but only %d are used in this edge clique certificate.\n", ekCert->nbEk, nbEkUsedInCert);
}

void displayEkCertR(EK_CERT_R* ekCert, bool displayZeroWeight)
{
	unsigned int i,j;
	unsigned int nbEkUsedInCert = 0;
	mpq_t zero;

	mpq_init(zero);
	for(i = 0 ; i < ekCert->nbEk ; i++)
	{
		for(j = 0 ; j < ekCert->nbEltPerEk ; j++)
		{
			if(displayZeroWeight || mpq_cmp(ekCert->weight[i], zero))
			{
				displayNuple(ekCert->ek[i] + j);
				if(j < ekCert->nbEltPerEk - 1)
					printf("---");
			}
		}
		if(displayZeroWeight || mpq_cmp(ekCert->weight[i], zero))
			gmp_printf("\tweight %Qd\n", ekCert->weight[i]);
		if(mpq_cmp(ekCert->weight[i], zero))
			nbEkUsedInCert++;
	}
	printf("There are %d edge cliques but only %d are used in this edge clique certificate.\n", ekCert->nbEk, nbEkUsedInCert);
	mpq_clear(zero);
}
