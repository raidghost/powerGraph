#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

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

unsigned long rankF2Mpz(MATRIX_MPZ *m)
{
	unsigned long i,j,max;
	unsigned long rank = 0;
	mpz_t tmp,tmp2,tmp3;

	mpz_init(tmp);
	mpz_init(tmp2);
	mpz_init(tmp3);

	//We start Gauss pivoting
	for(i = 0 ; i < m->nbRows ; i++)
	{
		max = i;
		//We look for the max.
		for(j = max+1 ; j < m->nbRows ; j++)
		{
			if(mpz_cmp(m->mat[j],m->mat[max]) > 0)
				max = j;
		}
		if(mpz_cmp_ui(m->mat[max],0) == 0)
			return rank;
		else if(max > i)
		{//We put the max on position i.
			mpz_set(tmp3,m->mat[i]);
			mpz_set(m->mat[i],m->mat[max]);
			mpz_set(m->mat[max],tmp3);
		}
		rank++;
		if(rank == m->nbRows || rank == m->nbColumns)
			return rank;

		//We xor every lines under line i that needs to be xored.
		for(j = i+1 ; j < m->nbRows ; j++)
		{
			mpz_xor(tmp,m->mat[j],m->mat[i]);
			if(mpz_cmp(tmp,m->mat[j]) < 0)
				mpz_xor(m->mat[j],m->mat[i],m->mat[j]);
		}
	}
	return rank;
}

MATRIX_MPZ matrix2Mpz(MATRIX* mat)
{//This function convert a matrix to a list of big intergers of same dimensions.
	unsigned long i,j;
	MATRIX_MPZ m;
	mpz_t* matMpz;
	mpz_t power2;
	mpz_t tmp;

	m.nbRows = mat->nbRows;
	m.nbColumns = mat->nbColumns;
	m.mat = NULL;

	mpz_init(power2);
	mpz_init(tmp);
	matMpz = (mpz_t*)malloc(mat->nbRows * sizeof(mpz_t));
	if(matMpz == NULL)
		NO_MEM_LEFT()
	for(i = 0 ; i < mat->nbRows ; i++)
	{
		mpz_init(matMpz[i]);
		//We set the line i
		unsigned long last1 = mat->nbColumns;
		mpz_set_ui(power2, 1);//We initate power2 to 2^0.
		for(j = mat->nbColumns ; j > 0 ; j--)
		{
			if(mat->mat[i][j-1])
			{//Si il y a un 1 ligne i et colonne j.
				mpz_ui_pow_ui(tmp, 2, last1-j);
				mpz_mul(power2, power2, tmp);
				mpz_add(matMpz[i],matMpz[i],power2);
				last1 = j;
			}
		}
	}
	m.mat = matMpz;
	return m;
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
