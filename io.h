#ifndef IO
#define IO
#include <stdio.h>

#include "structs.h"

int string2Int(const char *string);
GRAPH loadGraphFromFile(char *fileName);
void freeGraph(GRAPH g);

#endif
