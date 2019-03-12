#ifndef TOOLS
#define TOOLS

int string2Int(const char *string);
unsigned long binom(unsigned char n, unsigned char k);
unsigned long** binomAll(unsigned char n);
int lexQsort(void const *nuple1, void const *nuple2);
int nuplecmp(unsigned int *nuple1, unsigned int* nuple2, unsigned int n);
unsigned long sortDn(DN* dn, unsigned long begin, unsigned long end);
unsigned int*** subSequences(unsigned int list[], unsigned long length);
void quickSortDn(DN* dn);
unsigned long dichoSearchDN(const DN *dn, unsigned int *x, unsigned long begin, unsigned long end);

#endif
