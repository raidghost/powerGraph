#ifndef POWERGRAPH
#define POWERGRAPH

#include "structs.h"

PADIQUE padique(unsigned long n, unsigned char p);
unsigned int*** subSequences(unsigned int list[], unsigned long length);
bool testHn(const GRAPH* g, unsigned int nMin, unsigned int nMax);
DN generateDn(const GRAPH* g, unsigned int n);

#endif
