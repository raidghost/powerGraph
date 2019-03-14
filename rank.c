#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "structs.h"

#define ROW_LENGTH 1000000

unsigned long rankF2(MATRIX *mat)
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
		for(k = i ; k < mat->nbColumns ; k++)
		{
			for(j = i + 1 ; j < mat->nbRows ; j++)
			{
				if(mat->mat[j][firstNonZeroEntry] != 0)
				{//We have found a non zero coef so we must xor line j !
					for(k = 0 ; k < mat->nbColumns ; k++)
						mat->mat[j][k] = (mat->mat[j][k] + mat->mat[i][k]) % 2;
				}
			}
		}
	}
	return rank;
}

unsigned long rankZ(long **matrix, const unsigned long SIZE){
	unsigned long i,j,k,pivot;
	unsigned long rank = 0;
	long coef = 0;
	long *tmp = NULL;

	for(i = 0 ; i < SIZE ; i++){
		if(matrix[i][i] == 0){
			pivot = i;
			for(j = i+1 ; j < SIZE ; j++){
				if(matrix[j][i] != 0){
					pivot = j;
					break;
				}
			}
			if(pivot == i)
			/*We have already reached the rank.*/
				break;
			else{
				tmp = matrix[i];
				matrix[i] = matrix[pivot];
				matrix[pivot] = tmp;
				rank++;
			}
		}
		for(j = i+1 ; j < SIZE ; j++){
			if(matrix[j][i] != 0){
				coef = matrix[j][i] / matrix[i][i];
				for(k = i ; k < SIZE ; k++){
					matrix[j][k] += coef * matrix[i][k];
				}
			}
		}
	}
	return rank;
}
