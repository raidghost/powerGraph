#ifndef HOMOLOGY
#define HOMOLOGY

#include "structs.h"

unsigned int testHn(const GRAPH* g, const unsigned int nMax, const int field, const int verbose);
DN generateDn(const GRAPH* g, unsigned int n);
DN generateDn2(const GRAPH *g, unsigned int n);
unsigned int* subSequencesLengthMoins1(unsigned int list[], unsigned long length);
unsigned int*** subSequencesBoundedLength(unsigned int list[], unsigned long length, unsigned long subSeqMaxLength);
unsigned int** subSequencesFixedLength(unsigned int list[], unsigned long length, unsigned long subSeqLength);
void freeSubSequences(unsigned int*** subseqs, unsigned long length);

#endif
