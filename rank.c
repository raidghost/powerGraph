#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

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
	unsigned long i,j,k,l,pivotRow,rank,dimMin;
	mpq_t zero,coef,prodTmp;
	mpq_t* tmp = NULL;


	mpq_inits(zero,coef,prodTmp,NULL);

	if(mat->nbRows < mat->nbColumns)
		dimMin = mat->nbRows;
	else
		dimMin = mat->nbColumns;

	i = 0;
	j = 0;
	rank = 0;
	while(i < mat->nbRows && j < mat->nbColumns)
	{
		if(mpq_equal(mat->mat[i][j],zero))
		{//If mat[i][j] == 0, we have to find an other pivot. We try on column j.
			pivotRow = i;
			for(k = i+1 ; k < mat->nbRows ; k++)
			{
				if(!mpq_equal(mat->mat[k][j],zero))
				{//If mat[k][j] != 0
					pivotRow = k;
					break;
				}
			}
			if(pivotRow > i)
			{//We put the pivot line at position i.
				tmp = mat->mat[i];
				mat->mat[i] = mat->mat[pivotRow];
				mat->mat[pivotRow] = tmp;
			}
		}
		if(!mpq_equal(mat->mat[i][j],zero))
		{//Otherwise this line has only zeros.
			rank++;
			if(rank == dimMin)
				break;
			//We perform, when needed, the gaussian elimination.
			for(k = i+1 ; k < mat->nbRows ; k++)
			{
				if(!mpq_equal(mat->mat[k][j],zero))
				{
					mpq_div(coef,mat->mat[k][j],mat->mat[i][j]);
					for(l = j ; l < mat->nbColumns ; l++)
					{
						mpq_mul(prodTmp,coef,mat->mat[i][l]);
						mpq_sub(mat->mat[k][l],mat->mat[k][l],prodTmp);
					}
				}
			}
			//Since we have performed gaussian elimination, we increment both i and j (see below).
			i++;
		}
		//In any case we try 
		j++;
	}
	mpq_clears(zero,coef,prodTmp,NULL);
	return rank;
}
