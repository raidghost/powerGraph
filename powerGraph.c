#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "limits.h"
#include "io.h"
#include "tools.h"
#include "display.h"
#include "powerGraph.h"
#include "rank.h"

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

unsigned int testHn(const GRAPH* g, unsigned int nMax, int verbose)
{//This function tests until which nMin <= n <= nMax the hypothesis Hm holds for the graph g.
	bool goOn = true;
	unsigned long i,j,k;
	unsigned long rankMn = 0, rankMnMoins1 = 0;
	unsigned int n = 0;
	unsigned int*** subseq = NULL;
	DN dnMoins1, dn;
	MATRIX mN;
	mN.mat = NULL;

	if(nMax <= 1)
	//We assume the graph to be non empty and conected.
		return 1;

	printf("Début\n");
	dnMoins1 = generateDn(g, 1);
	rankMnMoins1 = 1;
	printf("fin\n");

	n = 1;
	while(n < nMax && goOn)
	{
		if(verbose)
			printf("On teste l'hypothèse H%d.\n\n", n+1);

		//We generate Dn and Dn-1
		dn = generateDn(g, n+1);
		if(dn.nbTuples == 0)
		//We cannot define Dn hence Mn so we abord.
			break;

		if(verbose >= 2)
		{
			printf("D%d := \n\n", n+1);
			displayDn(&dn);
			printf("D%d := \n\n", n);
			displayDn(&dnMoins1);
		}

		//We create the matrix Mn
		mN.nbRows = dnMoins1.nbTuples;
		mN.nbColumns = dn.nbTuples;
		mN.mat = (char**)malloc(mN.nbRows * sizeof(char*));
		if(mN.mat == NULL)
			NO_MEM_LEFT()
		for(i = 0 ; i < mN.nbRows ; i++)
		{
			mN.mat[i] = (char*)malloc(mN.nbColumns * sizeof(char));
			if(mN.mat[i] == NULL)
				NO_MEM_LEFT()
			for(j = 0 ; j < mN.nbColumns ; j++)
				mN.mat[i][j] = 0;
		}
		//It's important for Dn and Dn-1 to be lexicographically sorted !
		for(j = 0 ; j < mN.nbColumns ; j++)
		{
			//-----------------Il faudra libérer la mémoire utilisée par subseq !
			subseq = subSequences(dn.tuples[j], dn.n);
			//For every subsequence of length n-1
			for(i = 0 ; i < dn.n ; i++)
			{
				//Find the appropriate index
				/*printf("i = %ld et j = %ld\n", i, j);
				for(k = 0 ; k < n - 1 ; k++)
					printf("%d-", subseq[n - 1][i][k]);
				printf("\nOn fait mN.mat[%ld][%ld] = 1\n", a, j);*/
				mN.mat[dichoSearchDN(&dnMoins1, subseq[dn.n - 1][i], 0, dnMoins1.nbTuples)][j] = 1;
			}
			//We free subseq
			freeSubSequences(subseq, dn.n);
		}

		if(verbose >= 3)
		{
			displayMatrix(&mN);
			printf("\n\n");
		}
		//Beware, computing the rank changes the matrix !
		rankMn = rankF2(&mN);
		if(verbose)
			printf("Le rang de M%d vaut : %ld\n", n+1, rankMn);

		if(rankMn == dnMoins1.nbTuples - rankMnMoins1)
		{
			n++;
			rankMnMoins1 = rankMn;
		}
		else
			goOn = false;

		//We free dnMoins1
		for(k = 0 ; k < dnMoins1.nbTuples ; k++)
			free(dnMoins1.tuples[k]);
		free(dnMoins1.tuples);
		//We free mN
		for(i = 0 ; i < mN.nbRows ; i++)
			free(mN.mat[i]);
		free(mN.mat);

		dnMoins1 = dn;
	}

	//We free the useless memory.
	for(k = 0 ; k < dnMoins1.nbTuples ; k++)
		free(dnMoins1.tuples[k]);
	free(dnMoins1.tuples);

	return n;
}

DN generateDn(const GRAPH* g, unsigned int n)
{//Returns Dn lexicographically sorted.
	DN dn;
	dn.n = 0;
	dn.nbTuples = 0;
	dn.tuples = NULL;

	if(n == 1)
	{//We assume the graph to be connected.
		dn.n = 1;
		dn.nbTuples = g->nbVertices;
		dn.tuples = (unsigned int**)malloc(g->nbVertices * sizeof(unsigned int*));
		if(dn.tuples == NULL)
			NO_MEM_LEFT()
		unsigned long i;
		for(i = 0 ; i < g->nbVertices ; i++)
		{
			dn.tuples[i] = (unsigned int*)malloc(sizeof(unsigned int));
			if(dn.tuples[i] == NULL)
				NO_MEM_LEFT()
			dn.tuples[i][0] = i;
		}
		return dn;
	}
	
	unsigned long i,j,k,l,nbNeighbours;
	unsigned int* dnTmp[ARRAY_MAX_LENGTH];
	unsigned int *tuple, *tupleTmp;
	unsigned int*** subSeqTupleTmp = NULL;
	unsigned long nbTuples = 0;

	tuple = (unsigned int*)malloc(n * sizeof(unsigned int));
	if(tuple == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < g->nbVertices ; i++)
	{
//		printf("On en est à i = %ld\n", i);
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
					//The sequence has already been found.
						isSequenceNew = false;
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
	printf("peuplage de D%d\n", n);
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
	printf("Fin du peuplage de D%d. Il y a %ld éléments.\n", n, dn.nbTuples);
	if(dn.nbTuples >= 2)
	{
		printf("début du tri de D%d\n", n);
//		displayDn(&dn);
//		printf("\n\n");
//		unsigned long nbCalls = sortDn(dn, 0, dn.nbTuples - 1);
//		printf("fin du tri de D%d en %ld appels\n", n, nbCalls);
		quickSortDn(&dn);
		//displayDn(&dn);
//		fflush(stdout);
		printf("fin du tri de D%d\n", n);
	}
	return dn;
}

unsigned int*** subSequences(unsigned int list[], unsigned long length)
{//Computes all the strict subsequences of a list without doublons !
	unsigned int*** subseqs = NULL;
	unsigned int* newSubSeq = NULL;
	unsigned long i,j,k,l;
	unsigned long** binom = binomAll(length);

	subseqs = (unsigned int***)malloc(length * sizeof(unsigned int**));
	newSubSeq = (unsigned int*)malloc((length+1) * sizeof(unsigned int));

	if(subseqs == NULL || newSubSeq == NULL)
		NO_MEM_LEFT()
	
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
						while(l < i - 1 && list[k] > subseqs[i-1][j][l])
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

void freeSubSequences(unsigned int*** subseqs, unsigned long length)
{
	unsigned long i,j;
	unsigned long** binom = binomAll(length);
	for(i = 0 ; i < length ; i++)
	{
		for(j = 0 ; j < binom[length][i] ; j++)
			free(subseqs[i][j]);
	}
}
