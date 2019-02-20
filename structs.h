#ifndef STRUCTS
#define STRUCTS

typedef struct GRAPH GRAPH;
struct GRAPH{
	unsigned long nbVertices;
	char** mat;};

typedef struct PADIQUE PADIQUE;
struct PADIQUE{
	unsigned long nbBits;
	unsigned char* padique;};
#endif
