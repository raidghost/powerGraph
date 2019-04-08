#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io.h"
#include "tools.h"

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

unsigned long binom(unsigned int n, unsigned int k)
{//Computes n choose k
	unsigned long i,result;
	if(k > n)
		return 0;
	unsigned long** binom = binomAll(n);
	result = binom[n][k];
	//We free useless memory
	for(i = 0 ; i <= n ; i++)
		free(binom[i]);
	free(binom);
	return result;
}

unsigned long** binomAll(unsigned int n)
{//Computes Pascal's triangle given n.
	unsigned int i,j;
	unsigned long** binom = (unsigned long**)malloc((n+1) * sizeof(unsigned long*));
	if(!binom)
		NO_MEM_LEFT()
	for(i = 0 ; i <= n ; i++)
	{
		binom[i] = (unsigned long*)malloc((i+1) * sizeof(unsigned long));
		if(!binom[i])
			NO_MEM_LEFT()
	}
	binom[0][0] = 1;
	for(i = 1 ; i <= n ; i++)
	{
		binom[i][0] = 1;
		binom[i][1] = i;
		binom[i][i] = 1;
	}
	for(i = 1 ; i <= n ; i++)
	{
		for(j = 2 ; j < i ; j++)
			binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
	}
	return binom;
}

void freeBinomAll(unsigned long** bin, unsigned int n)
{
	unsigned int i;
	for(i = 0 ; i <= n ; i++)
		free(bin[i]);
	free(bin);
}

int nuplecmp(unsigned int *nuple1, unsigned int *nuple2, unsigned int n)
{//Returns 0 if nuple1 == nuple2, -1 if nuple1 < nuple2 and +1 if nuple1 > nuple2.
	unsigned int i;
	for(i = 0 ; i < n ; i++)
	{
		if(nuple1[i] < nuple2[i])
			return -1;
		else if(nuple1[i] > nuple2[i])
			return 1;
	}
	return 0;
}

int nupleCmp(const NUPLE *n1, const NUPLE *n2)
{//Returns 0 if n1 == n2, -1 if n1 < n2 and +1 if n1 > n2.
	unsigned int i, n = 0;
	if(n1->length < n2->length)
		n = n1->length;
	else
		n = n2->length;

	for(i = 0 ; i < n ; i++)
	{
		if(n1->tab[i] < n2->tab[i])
			return -1;
		else if(n1->tab[i] > n2->tab[i])
			return 1;
	}
	return 0;
}

void nupleCpy(const NUPLE *n1, NUPLE *n2)
{
	unsigned int i;
	n2->length = n1->length;
	for(i = 0 ; i < n2->length ; i++)
		n2->tab[i] = n1->tab[i];
}

int commonDegre(GRAPH* g)
{//Compute the common degree of all vertices of G. If there is no common degree, returns -1.
	if(g->nbVertices == 0)
		return 0;
	unsigned long i = 0, j;
	int result = 0, resultOld = 0;
	for(j = 0 ; j < g->nbVertices ; j++)
	{
		if(g->mat[i][j] == 1)
			resultOld++;
	}
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j] == 1)
			{
				if(result == resultOld)
					return -1;
				result++;
			}
		}
	}
	return result;
}

unsigned int nbEdges(GRAPH* g)
{
	unsigned int result = 0;
	unsigned long i,j;
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		for(j = i+1 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j] == 1)
				result++;
		}
	}
	return result;
}

void sortDn(DN* dn, unsigned long begin, unsigned long end)
{//Sort lexicographically an instance of DN using quicksort and returns the number of recursives calls used.
	unsigned int* tmp;
	if(end - begin <= 0)
		return;
	else if(end - begin == 1)
	{
		if(nuplecmp(dn->tuples[end],dn->tuples[begin],dn->n) < 0)
		{
			tmp = dn->tuples[begin];
			dn->tuples[begin] = dn->tuples[end];
			dn->tuples[end] = tmp;
		}
	}
	else
	{
		unsigned long i,pivot;
		pivot = end - begin;
		i = begin;
		while(i < begin + pivot && i < end)
		{
			if(nuplecmp(dn->tuples[begin + pivot],dn->tuples[i],dn->n) < 0)
			{
				tmp = dn->tuples[begin + pivot];
				if(i < begin + pivot - 1)
				{
					dn->tuples[begin + pivot] = dn->tuples[i];
					dn->tuples[i] = dn->tuples[begin + pivot - 1];
					dn->tuples[begin + pivot - 1] = tmp;
				}
				else
				{
					dn->tuples[begin + pivot] = dn->tuples[i];
					dn->tuples[i] = tmp;
				}
				pivot--;
			}
			else
				i++;
		}
		if(pivot >= 2)
			sortDn(dn, begin, begin + pivot - 1);
		if(end >= begin + pivot + 2)
			sortDn(dn, begin + pivot + 1, end);
	}
}

unsigned long dichoSearchDN(const DN *dn, unsigned int *x, unsigned long begin, unsigned long end)
{//Given a sorted DN, look for x in DN assuming x belongs to dn (undefined behavior if x is not in dn...)
	if(begin > end || end > dn->nbTuples)
		return 0;
	else
	{
		unsigned long midle = (end + begin) / 2;
		int cmp = nuplecmp(x, dn->tuples[midle], dn->n);
		switch(cmp)
		{
			case -1:
			return dichoSearchDN(dn, x, begin, midle - 1);
			break;

			case 1:
			return dichoSearchDN(dn, x, midle + 1, end);
			break;

			default:
			return midle;
		}
	}
}

long dichoSearchNupleList(const NUPLE *list, const NUPLE *n, unsigned long begin, unsigned long end)
{//Given a sorted list of nuples, we search for an element. If not found return -1.
	if(begin > end)
		return -1;
	unsigned long midle = (end + begin) / 2;
	int cmp = nupleCmp(n, list + midle);
	switch(cmp)
	{
		case -1:
		return dichoSearchNupleList(list, n, begin, midle - 1);
		break;

		case 1:
		return dichoSearchNupleList(list, n, midle + 1, end);
		break;

		default:
		return midle;
	}
}

void padiqueExpansion(NUPLE* nuple, unsigned long n, unsigned char p)
{//Given a non negative integer n, return n written in base p.
//	padiqueN.length = (unsigned int)floor(log(n)/log(p)) + 1;

	unsigned long i;
	unsigned long m, power;
	nuple->tab = (unsigned int*)realloc(nuple->tab, nuple->length * sizeof(unsigned int));
	if(nuple->tab == NULL)
		NO_MEM_LEFT()
	power = pow(p,nuple->length-1);
	m = nuple->length;
	for(i = nuple->length ; i > 0 ; i--)
	{
		nuple->tab[m-i] = n / power;
		n -= nuple->tab[m-i] * power;
		power = power / p;
	}
}
