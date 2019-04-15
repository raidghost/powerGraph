#ifndef DISPLAY
#define DISPLAY

#include "structs.h"

void displayGraph(const GRAPH* g);
void displayDn(const DN* dn);
void displayMatrixF2(const MATRIX_F2* m);
void displayMatrixR(const MATRIX_R* m);
void displayNuple(NUPLE* n);
void displayGraphList(GRAPH_LIST* g);
void displayEkCertF2(EK_CERT_F2* ekCert, bool displayZeroWeight);
void displayEkCertR(EK_CERT_R* ekCert, bool displayZeroWeight);

#endif
