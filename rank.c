#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "structs.h"

//#include "display.h"

#define ROW_LENGTH 1000000

unsigned long rankF2(MATRIX_F2 *mat)
{//This function computes the rank in F2 of a matrix AND CHANGES THIS MATRIX !
	unsigned long i,j,k,max;
	unsigned long rank = 0;
	unsigned long firstNonZeroEntry = 0;
	char *tmp = NULL;

	//We start Gauss pivoting
	for(i = 0 ; i < mat->nbRows ; i++)
	{
		//We look for the row corresponding to the maximum binary integer.
		max = i;
		for(j = max + 1 ; j < mat->nbRows ; j++)
		{
			for(k = 0 ; k < mat->nbColumns ; k++)
			{
				if(mat->mat[j][k] < mat->mat[max][k])
					break;
				else if(mat->mat[j][k] > mat->mat[max][k])
				{
					max = j;
					break;
				}
			}
		}
		//We test wether the maximum line is zero and retreive the first non zero entry.
		for(j = 0 ; j < mat->nbColumns ; j++)
		{
			if(mat->mat[max][j] != 0)
			{
				firstNonZeroEntry = j;
				break;
			}
		}
		if(j == mat->nbColumns)
		//If the maximum line is zero then we have the rank.
			return rank;
		else if(max > i)
		{//We put the max in position i.
			tmp = mat->mat[i];
			mat->mat[i] = mat->mat[max];
			mat->mat[max] = tmp;
		}
		rank++;
		if(rank == mat->nbRows || rank == mat->nbColumns)
			return rank;
		//We xor every line under line i that needs to be xored.
		for(j = i + 1 ; j < mat->nbRows ; j++)
		{
			if(mat->mat[j][firstNonZeroEntry] != 0)
			{//We have found a non zero coef so we must xor line j !
				for(k = 0 ; k < mat->nbColumns ; k++)
					mat->mat[j][k] = (mat->mat[j][k] + mat->mat[i][k]) % 2;
			}
		}
	}
	return rank;
}

unsigned long rankR(MATRIX_R* mat)
{
	unsigned long i,j,k,pivot,rank = 0,dimMin;
	mpq_t zero,coef,prodTmp;
	mpq_t* tmp = NULL;


	mpq_inits(zero,coef,prodTmp,NULL);
	if(mat->nbRows <= mat->nbColumns)
		dimMin = mat->nbRows;
	else
		dimMin = mat->nbColumns;

	for(i = 0 ; i < dimMin ; i++)
	{
		pivot = i;
		if(mpq_equal(mat->mat[i][i],zero))
		{//If mat[i][i] == 0, We have to find an other pivot.
			for(j = i+1 ; j < mat->nbRows ; j++)
			{
				if(!mpq_equal(mat->mat[j][i],zero))
				{//If mat[j][i] != 0
					pivot = j;
					break;
				}
			}
			if(pivot != i)
			{//We put the pivot line at position i.
				tmp = mat->mat[i];
				mat->mat[i] = mat->mat[pivot];
				mat->mat[pivot] = tmp;
			}
		}
		if(!mpq_equal(mat->mat[i][i],zero))
		{//Otherwise this line has only zeros.
			rank++;
			if(rank == dimMin)
				break;
			//We perform, when needed, the gaussian elimination.
			for(j = i+1 ; j < mat->nbRows ; j++)
			{
				if(!mpq_equal(mat->mat[j][i],zero))
				{
					mpq_div(coef,mat->mat[j][i],mat->mat[i][i]);
					for(k = i ; k < mat->nbColumns ; k++)
					{
						mpq_mul(prodTmp,coef,mat->mat[i][k]);
						mpq_sub(mat->mat[j][k],mat->mat[j][k],prodTmp);
					}
				}
			/*	displayMatrixR(mat);
				printf("\n\n");*/
			}
		}
	}
	mpq_clears(zero,coef,prodTmp,NULL);
	return rank;
}
