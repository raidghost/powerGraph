#ifndef STRUCTS
#define STRUCTS

#define TRUE 1
#define FALSE !TRUE

typedef enum {false, true} bool;
typedef struct GRAPH GRAPH;
struct GRAPH
{
	unsigned long nbVertices;
	char** mat;
};

typedef struct PADIQUE PADIQUE;
struct PADIQUE
{
	unsigned long nbBits;
	unsigned char* padique;
};

typedef struct MATRIX MATRIX;
struct MATRIX
{
	unsigned long nbRows;
	unsigned long nbColumns;
	char** mat;
};

#endif
