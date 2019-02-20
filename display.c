#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

void displayGraph(const GRAPH g){
	unsigned long i,j;
	for(i = 0 ; i < g.nbVertices ; i++){
		for(j = 0 ; j < g.nbVertices ; j++)
			printf("%d ", (int)g.mat[i][j]);
		printf("\n");
	}
}
