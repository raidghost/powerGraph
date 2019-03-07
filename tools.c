#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "tools.h"


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

bool lex(unsigned int* nuple1, unsigned int* nuple2, unsigned int n)
{//Returns true iff nuple1 < nuple2.
	unsigned int i;
	for(i = 0 ; i < n ; i++)
	{
		if(nuple1[i] < nuple2[i])
			return true;
		else if(nuple1[i] > nuple2[i])
			return false;
	}
	return false;
}
void sortDn(DN dn, unsigned long begin, unsigned long end)
{//Sort lexicographically an instance of DN using quicksort.
	unsigned int* tmp;
	if(end - begin <= 0)
		return;
	else if(end - begin == 1)
	{
		if(lex(dn.tuples[end],dn.tuples[begin],dn.n))
		{
			tmp = dn.tuples[begin];
			dn.tuples[begin] = dn.tuples[end];
			dn.tuples[end] = tmp;
		}
	}
	else
	{
		unsigned long i,pivot;
		pivot = rand() % (end - begin + 1);
		if(pivot < end - begin)
		{//If needed, we put the pivot at the end.
			tmp = dn.tuples[end];
			dn.tuples[end] = dn.tuples[begin + pivot];
			dn.tuples[begin + pivot] = tmp;
			pivot = end - begin;
		}
		i = begin;
		while(i < begin + pivot && i < end)
		{
			if(lex(dn.tuples[begin + pivot],dn.tuples[i],dn.n))
			{
				tmp = dn.tuples[begin + pivot];
				dn.tuples[begin + pivot] = dn.tuples[i];
				dn.tuples[i] = dn.tuples[begin + pivot - 1];
				dn.tuples[begin + pivot - 1] = tmp;
				pivot--;
			}
			else
				i++;
		}
		if(pivot >= begin + 2)
			sortDn(dn, begin, pivot - 1);
		if(end >= pivot + 2)
			sortDn(dn, pivot + 1, end);
	}
}
