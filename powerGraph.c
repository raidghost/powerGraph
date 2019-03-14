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

unsigned int testHn(const GRAPH* g, const unsigned int nMax, const int field, const int verbose)
{//This function tests until which nMin <= n <= nMax the hypothesis Hm holds for the graph g.
	if(field != 0 && field != 2)
		return 0;
	bool goOn = true;
	unsigned long i,j,k,l;
	unsigned long rankMn = 0, rankMnMoins1 = 0, nbRows, nbColumns;
	unsigned int n = 0;
	unsigned int* subseqs = NULL;
	DN dnMoins1, dn;
	MATRIX_F2 mNF2;
	mNF2.mat = NULL;
	MATRIX_R mNR;
	mNR.mat = NULL;

	if(nMax <= 1)
	//We assume the graph to be non empty and conected.
		return 1;

	dnMoins1 = generateDn(g, 1);
	rankMnMoins1 = 1;

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

		if(verbose >= 3)
		{
			printf("\nD%d := \n\n", n);
			displayDn(&dnMoins1);
			printf("\nD%d := \n\n", n+1);
			displayDn(&dn);
		}

		//We create the matrix Mn
		nbRows = dnMoins1.nbTuples;
		nbColumns = dn.nbTuples;
		switch(field)
		{
			case 0:
			mNR.nbRows = nbRows;
			mNR.nbColumns = nbColumns;
			mNR.mat = (long long**)malloc(mNR.nbRows * sizeof(long long*));
			if(mNR.mat == NULL)
				NO_MEM_LEFT()
			for(i = 0 ; i < mNR.nbRows ; i++)
			{
				mNR.mat[i] = (long long*)calloc(mNR.nbColumns,sizeof(long long));
				if(mNR.mat[i] == NULL)
					NO_MEM_LEFT()
			}
			break;

			case 2:
			mNF2.nbRows = dnMoins1.nbTuples;
			mNF2.nbColumns = dn.nbTuples;
			mNF2.mat = (char**)malloc(mNF2.nbRows * sizeof(char*));
			if(mNF2.mat == NULL)
				NO_MEM_LEFT()
			for(i = 0 ; i < mNF2.nbRows ; i++)
			{
				mNF2.mat[i] = (char*)calloc(mNF2.nbColumns,sizeof(char));
				if(mNF2.mat[i] == NULL)
					NO_MEM_LEFT()
			}
		}

		//It's important for Dn and Dn-1 to be lexicographically sorted !
		for(j = 0 ; j < nbColumns ; j++)
		{
			subseqs = subSequencesLengthMoins1(dn.tuples[j], dn.n);
			//For every subsequence of length n-1
			switch(field)
			{
				case 0://If the field is R, we have to deal with orientation.
				for(i = 0 ; i < dn.n ; i++)
				{
					//We find the first index in dn.tuples[j] but not in subseq[i].
					for(k = 0 ; k < dn.n ; k++)
					{
						for(l = 0 ; l < dn.n-1 ; l++)
						{
							if(subseqs[i*(dn.n-1) + l] == dn.tuples[j][k])
								break;
						}
						if(l == dn.n-1)
						{//If dn.tuples[i][k] is not in subseqs[i]

							if(k % 2 == 0)
								mNR.mat[dichoSearchDN(&dnMoins1, subseqs + i*(dn.n-1), 0, dnMoins1.nbTuples)][j] = 1;
							else
								mNR.mat[dichoSearchDN(&dnMoins1, subseqs + i*(dn.n-1), 0, dnMoins1.nbTuples)][j] = -1;
							break;
						}
					}
				}
				break;

				case 2:
				for(i = 0 ; i < dn.n ; i++)
						mNF2.mat[dichoSearchDN(&dnMoins1, subseqs + i*(dn.n-1), 0, dnMoins1.nbTuples)][j] = 1;
			}
			//We free subseq
			free(subseqs);
		}

		if(verbose >= 3)
		{
			printf("\nM%d :=\n", n+1);
			switch(field)
			{
				case 0:
				displayMatrixR(&mNR);
				break;

				case 2:
				displayMatrixF2(&mNF2);
			}
			printf("\n\n");
		}
		//Beware, computing the rank changes the matrix !
		if(verbose >= 2)
			printf("On calcule le rang de M%d qui est de taille %ld x %ld.\n", dn.n, nbRows, nbColumns);

		switch(field)
		{//We select the appropriate field.
			case 0:
			rankMn = rankR(&mNR);
			break;

			case 2:
			rankMn = rankF2(&mNF2);
		}
		if(verbose)
			printf("\nLe rang de M%d vaut : %ld\n", dn.n, rankMn);

		if(rankMn == dnMoins1.nbTuples - rankMnMoins1)
		{
			n++;
			rankMnMoins1 = rankMn;
			if(verbose)
				printf("\nH%d est vraie.\n", n);
		}
		else
		{
			if(verbose)
				printf("\nH%d est fausse.\n", n+1);
			goOn = false;
		}

		//We free dnMoins1
		for(k = 0 ; k < dnMoins1.nbTuples ; k++)
			free(dnMoins1.tuples[k]);
		free(dnMoins1.tuples);

		//We free mN
		switch(field)
		{
			case 0:
			for(i = 0 ; i < mNR.nbRows ; i++)
				free(mNR.mat[i]);
			free(mNR.mat);
			break;

			case 2:
			for(i = 0 ; i < mNF2.nbRows ; i++)
				free(mNF2.mat[i]);
			free(mNF2.mat);
		}
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

	bool isSequenceNew;
	unsigned long i,j,k,l,nbNeighbours;
	unsigned int dnTmp[ARRAY_MAX_LENGTH];
	unsigned int *tupleTmp = NULL;
	unsigned int** subSeqTupleTmp = NULL;
	unsigned long nbTuples = 0;

	tupleTmp = (unsigned int*)malloc(g->nbVertices * sizeof(unsigned int));
	if(tupleTmp == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < g->nbVertices ; i++)
	{
		nbNeighbours = 0;
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
			subSeqTupleTmp = subSequencesFixedLength(tupleTmp, nbNeighbours, n);
			//We now add, if needed, the new n-uples to dnTmp
			unsigned long** bin = binomAll(nbNeighbours);
			for(j = 0 ; j < bin[nbNeighbours][n] ; j++)
			{
				isSequenceNew = true;
				for(k = 0 ; k < nbTuples ; k++)
				{
					for(l = 0 ; l < n ; l++)
					{
						if(dnTmp[k * n + l] != subSeqTupleTmp[j][l])
							break;
					}
					if(l == n)
					{//The sequence has already been found.
						isSequenceNew = false;
						break;
					}
				}
				if(isSequenceNew)
				{
					for(k = 0 ; k < n ; k++)
						dnTmp[nbTuples * n + k] = subSeqTupleTmp[j][k];
					nbTuples++;
				}
			}
			//We free useless memory.
			for(k = 0 ; k < bin[nbNeighbours][n] ; k++)
				free(subSeqTupleTmp[k]);
			free(subSeqTupleTmp);
			freeBinomAll(bin, nbNeighbours);
		}
		//This else if could be done by the previous if by letting nbNeighbours >= n. However, for performance reason, we prefer not to call subSequencesFixedLength.
		else if(nbNeighbours == n)
		{
			//We add the new sequence if needed.
			isSequenceNew = true;
			for(k = 0 ; k < nbTuples ; k++)
			{
				for(l = 0 ; l < n ; l++)
				{
					if(dnTmp[k*n+l] != tupleTmp[l])
						break;
				}
				if(l == n)
				{//The sequence has already been found.
					isSequenceNew = false;
					break;
				}
			}
			if(isSequenceNew)
			{
				for(k = 0 ; k < n ; k++)
					dnTmp[nbTuples * n + k] = tupleTmp[k];
				nbTuples++;
			}
		}
	}
	free(tupleTmp);

	//We populate dn
	dn.n = n;
	dn.nbTuples = nbTuples;
	if(nbTuples == 0)
		return dn;
	dn.tuples = (unsigned int**)malloc(nbTuples * sizeof(unsigned int*));
	if(dn.tuples == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < nbTuples ; i++)
	{
		dn.tuples[i] = (unsigned int*)malloc(n * sizeof(unsigned int));
		if(dn.tuples[i] == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < n ; j++)
			dn.tuples[i][j] = dnTmp[i * n + j];
	}
	if(dn.nbTuples >= 2)
		sortDn(&dn, 0, dn.nbTuples - 1);
	return dn;
}

DN generateDn2(const GRAPH *g, unsigned int n)
{//This function uses another algorithm than generateDn to compute the same thing.
	DN dn;
	dn.n = n;
	dn.nbTuples = 0;
	dn.tuples = NULL;

	NUPLE currentNuple;
	currentNuple.length = n;

	if(n > g->nbVertices)
		return dn;

	//We compute an upper bound on the number of n-nuple dominated.
	unsigned long nbNupleMax = binom(g->nbVertices, n);
	unsigned long i,j,nupleNumber, nbNuples = 0;
	unsigned int *dnTmp = (unsigned int*)malloc(nbNupleMax * n * sizeof(unsigned int));

	if(dnTmp == NULL)
		NO_MEM_LEFT()
	//We try every possible nuple. One can obtain the corresponding nuple from its number by doing a padique extension.
	for(nupleNumber = 0 ; nupleNumber < nbNupleMax ; nupleNumber++)
	{
		padiqueExpansion(&currentNuple, nupleNumber, g->nbVertices);
		//We test if the current nuple is dominated in the graph.
		bool dominated = false;
		for(j = 0 ; j < g->nbVertices ; j++)
		{
			bool dominatedByJ = true;
			for(i = 0 ; i < currentNuple.length ; i++)
			{
				if(!g->mat[currentNuple.tab[i]][j])
				{
					dominatedByJ = false;
					break;
				}
			}
			if(dominatedByJ)
			{
				dominated = true;
				break;
			}
		}
		if(dominated)
		{//If needed, we add the current nuple to dn.
			for(i = 0 ; i < n ; i++)
				dnTmp[nbNuples * n + i] = currentNuple.tab[i];
			nbNuples++;
		}
	}

	//We populate dn.
	dn.nbTuples = nbNuples;
	dn.tuples = (unsigned int**)malloc(nbNuples * sizeof(unsigned int*));
	if(dn.tuples == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < nbNuples ; i++)
	{
		dn.tuples[i] = (unsigned int*)malloc(n * sizeof(unsigned int));
		if(dn.tuples == NULL)
			NO_MEM_LEFT()
		for(j = 0 ; j < n ; j++)
			dn.tuples[i][j] = dnTmp[i * n + j];
	}

	//We free useless memory.
	free(currentNuple.tab);
	free(dnTmp);
	return dn;
}

unsigned int* subSequencesLengthMoins1(unsigned int list[], unsigned long length)
{//Compute all the subsequences of length length - 1
//It's much faster than calling the function subSequences and use only a small part of the result !
	if(length == 0)
		return NULL;
	unsigned int* subseqs = (unsigned int*)malloc(length * (length - 1) * sizeof(unsigned int));
	if(subseqs == NULL)
		NO_MEM_LEFT()
	unsigned i,j,k;
	for(i = 0 ; i < length ; i++)
	{
		k = 0;
		for(j = 0 ; j < length ; j++)
		{
			if(j != i)
			{
				subseqs[i * (length-1) + k] = list[j];
				k++;
			}
		}
	}
	return subseqs;
}

unsigned int** subSequencesFixedLength(unsigned int list[], unsigned long length, unsigned long subSeqLength)
{//Computes all the subsequences of lenght = subSeqMaxLength of a list without doublons !
	if(length == 0 || subSeqLength > length)
		return NULL;
	unsigned int*** subseqs;
	unsigned int** result = NULL;
	unsigned int* newSubSeq = NULL;
	unsigned long i,j,k,l;
	unsigned long** bin = binomAll(length);

	subseqs = (unsigned int***)malloc(subSeqLength* sizeof(unsigned int**));

	if(subseqs == NULL)
		NO_MEM_LEFT()
	
	//We can now create the subseqs array of array with good size using binomials.
	for(i = 0 ; i < subSeqLength ; i++)
	{
		//We know exactly the number of subsequences of size i+1.
		subseqs[i] = (unsigned int**)malloc(bin[length][i+1] * sizeof(unsigned int*));
		if(subseqs[i] == NULL)
			NO_MEM_LEFT()
		if(i == 0)
		{
			for(j = 0 ; j < length ; j++)
			{
				subseqs[0][j] = (unsigned int*)malloc(sizeof(unsigned int));
				if(subseqs[0][j] == NULL)
					NO_MEM_LEFT()
				subseqs[0][j][0] = list[j];
			}
		}
		else
		{
			for(j = 0 ; j < bin[length][i+1] ; j++)
			{
				subseqs[i][j] = (unsigned int*)malloc((i+1) * sizeof(unsigned int));
				if(subseqs[i][j] == NULL)
					NO_MEM_LEFT()
			}
			//We look at every subsequences of size i.
			unsigned int nbNewSubSeqs = 0;
			for(j = 0 ; j < bin[length][i] ; j++)
			{
				//We look for an element which is not in subseqs[i-1][j].
				for(k = 0 ; k < length ; k++)
				{
					for(l = 0 ; l < i ; l++)
					{
						if(list[k] == subseqs[i-1][j][l])
							break;
					}
					if(l == i)
					{//list[k] is not in subseqs[i-1][j]
						newSubSeq = (unsigned int*)realloc(newSubSeq, (i+1) * sizeof(unsigned int));
						if(newSubSeq == NULL)
							NO_MEM_LEFT()
						//Now we insert list[k] at the appropriate position in subseqs[i-1][j]
						l = 0;
						while(l < i && list[k] > subseqs[i-1][j][l])
						{
							newSubSeq[l] = subseqs[i-1][j][l];
							l++;
						}
						newSubSeq[l] = list[k];
						l++;
						while(l < i+1)
						{
							newSubSeq[l] = subseqs[i-1][j][l-1];
							l++;
						}

						//We must now check that the sequence obtained by adding list[k] to subseqs[i-1][j] (that is newSubSeq) is not already in subseqs[i].
						bool subSeqExists = false;
						unsigned a,b;
						for(a = 0 ; a < nbNewSubSeqs ; a++)
						{
							for(b = 0 ; b < i+1 ; b++)
							{
								if(subseqs[i][a][b] != newSubSeq[b])
									break;
							}
							if(b == i+1)
							{//If the subsequences are the same
								subSeqExists = true;
								break;
							}
						}
						if(!subSeqExists)
						{
							for(l = 0 ; l < i+1 ; l++)
								subseqs[i][nbNewSubSeqs][l] = newSubSeq[l];
							nbNewSubSeqs++;
						}
					}
				}
				free(subseqs[i-1][j]);
			}
			free(subseqs[i-1]);
		}
	}
	freeBinomAll(bin, length);
	result = subseqs[subSeqLength-1];
	free(subseqs);
	free(newSubSeq);
	return result;
}


unsigned int*** subSequencesBoundedLength(unsigned int list[], unsigned long length, unsigned long subSeqMaxLength)
{//Computes all the strict subsequences of lenght <= subSeqMaxLength of a list without doublons !
	if(length == 0 || subSeqMaxLength > length)
		return NULL;
	unsigned int*** subseqs;
	unsigned int* newSubSeq;
	unsigned long i,j,k,l;
	unsigned long** bin = binomAll(length);

	subseqs = (unsigned int***)malloc((subSeqMaxLength+1)* sizeof(unsigned int**));

	if(subseqs == NULL)
		NO_MEM_LEFT()
	
	//We can now create the subseqs array of array with good size using binomials.
	for(i = 0 ; i <= subSeqMaxLength ; i++)
	{
		//We know exactly the number of subsequences of size i.
		subseqs[i] = (unsigned int**)malloc(bin[length][i] * sizeof(unsigned int*));
		if(subseqs[i] == NULL)
			NO_MEM_LEFT()
		if(i == 0)
		{
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
			for(j = 0 ; j < bin[length][i] ; j++)
			{
				subseqs[i][j] = (unsigned int*)malloc(i * sizeof(unsigned int));
				if(subseqs[i][j] == NULL)
					NO_MEM_LEFT()
			}
			//We look at every subsequences of size i-1.
			unsigned int nbNewSubSeqs = 0;
			for(j = 0 ; j < bin[length][i-1] ; j++)
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
						newSubSeq = (unsigned int*)malloc(i * sizeof(unsigned int));
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
						unsigned a,b;
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
						else
							free(newSubSeq);
					}
				}
			}
		}
	}
	freeBinomAll(bin, length);
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
