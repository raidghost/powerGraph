#ifndef TOOLS
#define TOOLS

int string2Int(const char *string);
unsigned long binom(unsigned char n, unsigned char k);
unsigned long** binomAll(unsigned char n);
bool lex(unsigned int *nuple1, unsigned int* nuple2, unsigned int n);
int nuplecmp(unsigned int *nuple1, unsigned int* nuple2, unsigned int n);
void sortDn(DN dn, unsigned long begin, unsigned long end);
unsigned int*** subSequences(unsigned int list[], unsigned long length);
unsigned long dichoSearchDN(const DN *dn, unsigned int *x, unsigned long begin, unsigned long end);

#endif
