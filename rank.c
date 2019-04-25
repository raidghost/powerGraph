#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

#define ROW_LENGTH 1000000

unsigned long rankF2(MATRIX_F2 *mat)
{//This function computes the rank in F2 of a matrix AND CHANGES THIS MATRIX !
	unsigned long i, j, k, l, pivotRow, dimMin, rank;
	char *tmp = NULL;

	if(mat->nbRows < mat->nbColumns)
		dimMin = mat->nbRows;
	else
		dimMin = mat->nbColumns;

	//We start Gauss pivoting.
	i = 0;
	j = 0;
	rank = 0;
	while(i < mat->nbRows && j < mat->nbColumns)
	{
		if(!mat->mat[i][j])
		{//We look for an other pivot.
			pivotRow = i;
			//We try on column j.
			for(k = i+1 ; k < mat->nbRows ; k++)
			{
				if(mat->mat[k][j])
				{
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
		if(mat->mat[i][j])
		{//Otherwise this line has only zeros.
			rank++;
			if(rank == dimMin)
				break;
			//We perform, when needed, the gaussian elimination.
			for(k = i+1 ; k < mat->nbRows ; k++)
			{
				if(mat->mat[k][j])
				{
					for(l = j ; l < mat->nbColumns ; l++)
						mat->mat[k][l] = (mat->mat[k][l] + mat->mat[i][l]) % 2;
				}
			}
			//Since we have performed gaussian elimination, we increment both i and j (see below).
			i++;
		}
		j++;
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
		j++;
	}
	mpq_clears(zero,coef,prodTmp,NULL);
	return rank;
}
