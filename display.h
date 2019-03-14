#ifndef DISPLAY
#define DISPLAY

#include <stdio.h>
#include <gmp.h>

void displayGraph(const GRAPH* g);
void displayDn(const DN* dn);
void displayMatrix(const MATRIX* m);
void printMatrixMpz(const MATRIX_MPZ* m);

#endif
