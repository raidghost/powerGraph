#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io.h"
#include "tools.h"

//Variables globales indispensables pour le quicksort.
unsigned int n1Length = 0;
unsigned int n2Length = 0;

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

unsigned long binom(unsigned char n, unsigned char k)
{//Computes n choose k
	if(k > n)
		return 0;
	unsigned long** binom = binomAll(n);
	return binom[n][k];
}

unsigned long** binomAll(unsigned char n)
{//Computes Pascal's triangle given n.
	unsigned char i,j;
	unsigned long** binom = NULL;

	binom = (unsigned long**)malloc((n+1) * sizeof(unsigned long*));
	if(binom == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i <= n ; i++)
	{
		binom[i] = (unsigned long*)malloc((i+1) * sizeof(unsigned long));
		if(binom[i] == NULL)
			NO_MEM_LEFT()
	}
	for(i = 0 ; i <= n ; i++)
	{
		binom[i][0] = 1;
		binom[i][1] = i;
		binom[i][i] = 1;
	}
	for(i = 1 ; i <= n ; i++)
	{
		for(j = 1 ; j < i ; j++)
			binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
	}
	return binom;
}

int lexQsort(void const *nuple1, void const *nuple2)
{
	NUPLE* n1 = (NUPLE*)nuple1;
	NUPLE* n2 = (NUPLE*)nuple2;
	unsigned int i, lengthMax = 0;
	if(n1->length > n2->length)
		lengthMax = n1->length;
	else
		lengthMax = n2->length;
	for(i = 0 ; i < lengthMax ; i++)
	{
		if(n1->tab[i] < n2->tab[i])
			return -1;
		else if(n1->tab[i] > n2->tab[i])
			return 1;
	}
	if(n1->length == n2->length)
		return 0;
	else if(n1->length < n2->length)
		return -1;
	else
		return 1;
}
/*int lexQsort(void const *nuple1, void const *nuple2)
{//This function relies on two global variables n1Length and n2Length that must be set before calling it.
	unsigned int* n1 = (unsigned int*)nuple1;
	unsigned int* n2 = (unsigned int*)nuple2;
	unsigned int i, lengthMin = 0;
	if(n1Length > n2Length)
		lengthMin = n2Length;
	else
		lengthMin = n1Length;
	for(i = 0 ; i < lengthMin ; i++)
	{
		if(n1[i] < n2[i])
			return -1;
		else if(n1[i] > n2[i])
			return 1;
	}
	if(n1Length == n2Length)
		return 0;
	else if(n1Length < n2Length)
		return -1;
	else
		return 1;
}*/


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

unsigned long sortDn(DN* dn, unsigned long begin, unsigned long end)
{//Sort lexicographically an instance of DN using quicksort and returns the number of recursives calls used.
	static unsigned long nbCalls = 0;
	nbCalls++;
//	printf("Appel %ld\n", nbCalls);
//	fflush(stdout);
	unsigned int* tmp;
	if(end - begin <= 0)
		return nbCalls;
	else if(end - begin == 1)
	{
		if(nuplecmp(dn->tuples[end],dn->tuples[begin],dn->n) < 0)
		{
			tmp = dn->tuples[begin];
			dn->tuples[begin] = dn->tuples[end];
			dn->tuples[end] = tmp;
		}
		return nbCalls;
	}
	else
	{
		unsigned long i,pivot;
		//pivot = rand() % (end - begin + 1);
		pivot = end - begin;
		/*if(pivot < end - begin)
		{//If needed, we put the pivot at the end.
			tmp = dn->tuples[end];
			dn->tuples[end] = dn->tuples[begin + pivot];
			dn->tuples[begin + pivot] = tmp;
			pivot = end - begin;
		}*/
		i = begin;
		while(i < begin + pivot && i < end)
		{
			//printf("%ld-", i);
			//fflush(stdout);
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
	return nbCalls;
}

void quickSortDn(DN* dn)
{
	//qsort((void*)*dn->tuples, sizeof(dn->tuples) / sizeof(*dn->tuples), sizeof(*dn->tuples), lexQsort);
	//qsort((void*)*dn->tuples, sizeof(dn->tuples) / sizeof(*dn->tuples), dn->n * sizeof(unsigned int), lexQsort);
//	n1Length = dn->n;
//	n2Length = dn->n;
	//qsort((void*)*dn->tuples, dn->nbTuples, dn->n * sizeof(unsigned int), lexQsort);
	//qsort((void*)dn->tuples, (size_t)dn->nbTuples, (size_t)(dn->n * sizeof(unsigned int)), lexQsort);
//	qsort((void*)dn->tuples, (size_t)dn->nbTuples, (size_t)(dn->n * sizeof(unsigned int)), lexQsort);

	NUPLE* dnTmp;
	dnTmp = (NUPLE*)malloc(dn->n * sizeof(NUPLE));
	if(dnTmp == NULL)
		NO_MEM_LEFT()
	unsigned long i,j;
	printf("Bienvenue dans quicksort.\n");
	fflush(stdout);
	for(i = 0 ; i < dn->nbTuples ; i++)
	{
		dnTmp[i].length = dn->n;
		dnTmp[i].tab = (unsigned int*)malloc(dn->n * sizeof(unsigned int));
		if(dnTmp[i].tab == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < dn->n ; j++)
		{
			dnTmp[i].tab[j] = dn->tuples[i][j];
			printf("%d--", dn->tuples[i][j]);
		}
		printf("\n");
	}
	//qsort((void*)dnTmp, (size_t)dn->nbTuples, sizeof(NUPLE), lexQsort);
	for(i = 0 ; i < dn->nbTuples ; i++)
	{
		for(j = 0 ; j < dn->n ; j++)
		{
			printf("%d-", dnTmp[i].tab[j]);
			if(dnTmp[i].tab[j] != dn->tuples[i][j])
				printf("BUG");
		}
		printf("\n");
	}
	printf("\n\n");
	/*for(i = 0 ; i < dn->nbTuples ; i++)
	{
		for(j = 0 ; j < dn->n ; j++)
			dn->tuples[i][j] = dnTmp[i].tab[j];
		free(dnTmp[i].tab);
	}
	free(dnTmp);*/
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

			//case 0:
			default:
			return midle;
		}
	}
}
