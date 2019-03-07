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
