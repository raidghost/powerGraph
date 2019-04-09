#ifndef HOMOLOGY
#define HOMOLOGY

#include "structs.h"

unsigned int testHn(const GRAPH* g, const int nMax, const int field, const int verbose);
DN generateDn(const GRAPH* g, unsigned int n);
unsigned int* subSequencesLengthMoins1(unsigned int list[], unsigned long length);
unsigned int** subSequencesFixedLength(unsigned int list[], unsigned long length, unsigned long subSeqLength);

#endif
