#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

PADIQUE padique(unsigned long n, unsigned char p);
unsigned int testHn(const GRAPH* g, unsigned int nMax);
DN generateDn(const GRAPH* g, unsigned int n);
unsigned int*** subSequences(unsigned int list[], unsigned long length);
void freeSubSequences(unsigned int*** subseqs, unsigned long length);

#endif
