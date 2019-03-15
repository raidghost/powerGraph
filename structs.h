#ifndef STRUCTS
#define STRUCTS

#include <gmp.h>

typedef enum {false, true} bool;
typedef struct GRAPH GRAPH;
struct GRAPH
{
	unsigned long nbVertices;
	char** mat;
};

typedef struct MATRIX_F2 MATRIX_F2;
struct MATRIX_F2
{
	unsigned long nbRows;
	unsigned long nbColumns;
	char** mat;
};

typedef struct MATRIX_R MATRIX_R;
struct MATRIX_R
{
	unsigned long nbRows;
	unsigned long nbColumns;
	mpq_t** mat;
};

typedef struct DN DN;
struct DN
{
	unsigned int n;
	unsigned long nbTuples;
	unsigned int** tuples;
};

typedef struct NUPLE NUPLE;
struct NUPLE
{
	unsigned int length;
	unsigned int* tab;
};

#endif
