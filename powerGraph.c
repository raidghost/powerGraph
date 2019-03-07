#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "limits.h"
#include "io.h"
#include "tools.h"
#include "powerGraph.h"

PADIQUE padique(unsigned long n, unsigned char p)
{
	/*Given a non negative integer n, return n written in base p*/
	unsigned long i;
	unsigned long m = 0, power = 0;
	PADIQUE padiqueN;
	padiqueN.nbBits = (unsigned long)floor(log(n)/log(p)) + 1;

	padiqueN.padique = (unsigned char*)malloc(padiqueN.nbBits * sizeof(unsigned char));
	if(padiqueN.padique == NULL)
	{
		fprintf(stderr, "No memory left.\n");
		exit(EXIT_FAILURE);
	}
	if(padiqueN.nbBits == 0)
	{
		padiqueN.padique = NULL;
		return padiqueN;
	}
	else if(padiqueN.nbBits == 1)
	{
		padiqueN.padique[0] = n;
		return padiqueN;
	}
	else
	{
		power = pow(p,padiqueN.nbBits-1);
		for(i = padiqueN.nbBits ; i > 0 ; i--)
		{
			padiqueN.padique[m-i] = n / power;
			n -= padiqueN.padique[m-i] * power;
			power = power / p;
		}
	return padiqueN;
	}
}


unsigned int*** subSequences(unsigned int list[], unsigned long length)
{//Computes all the strict subsequences of a list without doublons !
	unsigned int*** subseqs = NULL;
	unsigned int* newSubSeq = NULL;
	unsigned long i,j,k,l;
	unsigned long** binom = NULL;

	subseqs = (unsigned int***)malloc(length * sizeof(unsigned int**));
	binom = (unsigned long**)malloc((length+1) * sizeof(unsigned long*));
	newSubSeq = (unsigned int*)malloc((length+1) * sizeof(unsigned int));

	if(subseqs == NULL || binom == NULL || newSubSeq == NULL)
		NO_MEM_LEFT()
	//We compute the usefull binomial coefficients.
	for(i = 0 ; i <= length ; i++)
	{
		binom[i] = (unsigned long*)malloc((i+1) * sizeof(unsigned long));
		if(binom[i] == NULL)
			NO_MEM_LEFT()
	}

	for(i = 0 ; i <= length ; i++)
	{
		binom[i][0] = 1;
		binom[i][1] = i;
		binom[i][i] = 1;
	}
	for(i = 1 ; i <= length ; i++)
	{
		for(j = 1 ; j < i ; j++)
			binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
	}
	
	//We can now create the subseqs array of array with good size using binomials.
	for(i = 0 ; i < length ; i++)
	{
		//We know exactly the number of subsequences of size i.
		subseqs[i] = (unsigned int**)malloc(binom[length][i] * sizeof(unsigned int*));
		if(subseqs[i] == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < binom[length][i] ; j++)
		{
			subseqs[i][j] = (unsigned int*)malloc((i+1) * sizeof(unsigned int));
			if(subseqs[i][j] == NULL)
				NO_MEM_LEFT()
		}

		if(i == 0)
		{//There is one subsequence of size 0 : the empty subsequence.
			subseqs[0] = (unsigned int**)malloc(sizeof(unsigned int*));
			if(subseqs[0] == NULL)
				NO_MEM_LEFT()
			subseqs[0][0] = NULL;
		}
		else if(i == 1)
		{
			subseqs[1] = (unsigned int**)malloc(length * sizeof(unsigned int*));
			if(subseqs[1] == NULL)
				NO_MEM_LEFT()
			for(j = 0 ; j < length ; j++)
			{
				subseqs[1][j] = (unsigned int*)malloc(sizeof(unsigned int));
				if(subseqs[1][j] == NULL)
					NO_MEM_LEFT()
				subseqs[1][j][0] = list[j];
			}
		}
		else
		{
			//We look at every subsequences of size i-1.
			unsigned int nbNewSubSeqs = 0;
			for(j = 0 ; j < binom[length][i-1] ; j++)
			{
				//We look for an element which is not in subseqs[i-1][j].
				for(k = 0 ; k < length ; k++)
				{
					for(l = 0 ; l < i-1 ; l++)
					{
						if(list[k] == subseqs[i-1][j][l])
							break;
					}
					if(l == i-1)
					{//list[k] is not in subseqs[i-1][j]
						unsigned int a,b;
						newSubSeq = (unsigned int*)malloc((i+1) * sizeof(unsigned int));
						if(newSubSeq == NULL)
							NO_MEM_LEFT()
						//Now we insert list[k] at the appropriate position in subseqs[i-1][j]
						l = 0;
						while(list[k] < subseqs[i-1][j][l])
						{
							newSubSeq[l] = subseqs[i-1][j][l];
							l++;
						}
						newSubSeq[l] = list[k];
						l++;
						while(l < i)
						{
							newSubSeq[l] = subseqs[i-1][j][l-1];
							l++;
						}

						//We must now check that the sequence obtained by adding list[k] to subseqs[i-1][j] (that is newSubSeq) is not already in subseqs[i].
						bool subSeqExists = false;
						for(a = 0 ; a < nbNewSubSeqs ; a++)
						{
							for(b = 0 ; b < i ; b++)
							{
								if(subseqs[i][a][b] != newSubSeq[b])
									break;
							}
							if(b == i)
							{//If the subsequences are the same
								subSeqExists = true;
								break;
							}
						}
						if(!subSeqExists)
						{
							subseqs[i][nbNewSubSeqs] = newSubSeq;
							nbNewSubSeqs++;
						}
					}
				}
			}
		}
	}
/*	for(i = 0 ; i < length ; i++)
	{
		for(j = 0 ; j < binom[length][i] ; j++)
		{
			for(k = 0 ; k < i ; k++)
				printf("%d-", subseqs[i][j][k]);
			printf("\n");
		}
		printf("\n\n");
	}*/
	return subseqs;
}

/*bool testHn(GRAPH* g, unsigned int nMax, unsigned int nMin)
{//This function tests until which nMin <= n <= nMax the hypothesis Hm holds for the graph g.
	unsigned long i,j;
	unsigned int n = 2, m = 0;
	unsigned int* listDom = NULL;
	unsigned int*** subSeqListDom = NULL;
	unsigned long tnMoins1[ARRAY_MAX_LENGTH] = {0}, tn[ARRAY_MAX_LENGTH] = {0};
	unsigned long tnMoins1Length = 0, tnLength = 0, listDomLength = 0;

	if(g->nbVertices > ARRAY_MAX_LENGTH)
	{
		fprintf(stderr, "Consider raising ARRAY_MAX_LENGTH.\n");
		exit(EXIT_FAILURE);
	}
	
	listDom = (unsigned int*)malloc(g->nbVertices * sizeof(unsigned int));
	if(listDom == NULL)
	{
		fprintf(stderr, "No memory left.\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j])
			{
				listDom[listDomLength] = j;
				m++;
			}
		}
		if(m >= 2)
		{
			subSeqListDom = subSequences(listDom, listDomLength);
		}
	}
	return true;
}*/

bool testHn(const GRAPH* g, unsigned int nMin, unsigned int nMax)
{//This function tests until which nMin <= n <= nMax the hypothesis Hm holds for the graph g.
	unsigned long i,j,k;
	unsigned int n;
	DN dnMoins1, dn;

	if(nMax <= 1)
	//We assume the graph to be non empty and conected.
		return true;
	if(nMin < 2)
		nMin = 2;

	for(n = nMin ; n < nMax ; n++)
	{
		dn = generateDn(g, n+1);
		dnMoins1 = generateDn(g, n);
		displayDn(&dn);
		displayDn(&dnMoins1);
	}

	return true;
}

DN generateDn(const GRAPH* g, unsigned int n)
{
	unsigned long i,j,k,l,nbNeighbours;
	unsigned int* dnTmp[ARRAY_MAX_LENGTH];
	unsigned int *tuple, *tupleTmp;
	unsigned int*** subSeqTupleTmp = NULL;
	unsigned long nbTuples = 0;
	DN dn;
	dn.n = 0;
	dn.nbTuples = 0;
	dn.tuples = NULL;

	tuple = (unsigned int*)malloc(n * sizeof(unsigned int));
	if(tuple == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		nbNeighbours = 0;
		tupleTmp = (unsigned int*)malloc(g->nbVertices * sizeof(unsigned int));
		if(tupleTmp == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			if(g->mat[i][j] == 1)
			{
				tupleTmp[nbNeighbours] = j;
				nbNeighbours++;
			}
		}
		if(nbNeighbours > n)
		{//If vertex i has strictly more than n neighbours then we have to find all the subsequences of length n of tupleTmp and add it (if not exists) in dnTmp.
			subSeqTupleTmp = subSequences(tupleTmp, nbNeighbours);
			//We now add, if needed, the new n-uples to dnTmp
			unsigned long** bin = binomAll(nbNeighbours);
			for(j = 0 ; j < bin[nbNeighbours][n] ; j++)
			{
				bool isSequenceNew = true;
				for(k = 0 ; k < nbTuples ; k++)
				{
					for(l = 0 ; l < n ; l++)
					{
						if(dnTmp[k][l] != subSeqTupleTmp[n][j][l])
							break;
					}
					if(l == n)
					{//The sequence has already been found.
						isSequenceNew = false;
					}
				}
				if(isSequenceNew)
				{
					dnTmp[nbTuples] = subSeqTupleTmp[n][j];
					nbTuples++;
				}
			}
			//We free useless memory
			for(j = 0 ; j < n - 1 ; j++)
			{
				for(k = 0 ; k < bin[n][j] ; k++)
					free(subSeqTupleTmp[j][k]);
			}
			free(tupleTmp);

		}
		else if(nbNeighbours == n)
		{
			dnTmp[nbTuples] = tupleTmp;
			nbTuples++;
		}
		else
			free(tupleTmp);
	}

	//We populate dn
	dn.n = n;
	dn.nbTuples = nbTuples;
	dn.tuples = (unsigned int**)malloc(nbTuples * sizeof(unsigned int*));
	if(dn.tuples == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < nbTuples ; i++)
	{
		dn.tuples[i] = (unsigned int*)malloc(n * sizeof(unsigned int));
		if(dn.tuples[i] == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < n ; j++)
			dn.tuples[i][j] = dnTmp[i][j];
	}
	return dn;
}
