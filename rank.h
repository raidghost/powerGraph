#ifndef RANK
#define RANK

#include <gmp.h>

unsigned long rankF2(MATRIX *matrix);
MATRIX_MPZ matrix2Mpz(MATRIX* mat);
unsigned long rankF2Mpz(MATRIX_MPZ *m);
unsigned long rankZ(long **matrix, const unsigned long SIZE);

#endif
