#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

PADIQUE padique(unsigned long n, unsigned char p);
unsigned int** subSequences(unsigned int list[], unsigned long length);
bool testHn(GRAPH g, unsigned int nMax, unsigned int nMin);

#endif
