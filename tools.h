#ifndef TOOLS
#define TOOLS

#include "structs.h"

int string2Int(const char *string);
unsigned long binom(unsigned int n, unsigned int k);
unsigned long** binomAll(unsigned int n);
void freeBinomAll(unsigned long** bin, unsigned int n);
int nuplecmp(unsigned int *nuple1, unsigned int* nuple2, unsigned int n);
int nupleCmp(NUPLE *n1, NUPLE *n2);
void sortDn(DN* dn, unsigned long begin, unsigned long end);
unsigned int*** subSequences(unsigned int list[], unsigned long length);
unsigned long dichoSearchDN(const DN *dn, unsigned int *x, unsigned long begin, unsigned long end);
void padiqueExpansion(NUPLE* nuple, unsigned long n, unsigned char p);

#endif
