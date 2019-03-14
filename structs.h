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

typedef struct MATRIX MATRIX;
struct MATRIX
{
	unsigned long nbRows;
	unsigned long nbColumns;
	char** mat;
};

typedef struct MATRIX_MPZ MATRIX_MPZ;
struct MATRIX_MPZ
{
	unsigned long nbRows;
	unsigned long nbColumns;
	mpz_t* mat;
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
