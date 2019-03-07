#ifndef IO
#define IO
#include <stdio.h>

#include "structs.h"

#define NO_MEM_LEFT() {fprintf(stderr, "No memory left.\n");exit(EXIT_FAILURE);}
int string2Int(const char *string);
GRAPH loadGraphFromFile(char *fileName);
void freeGraph(GRAPH* g);

#endif
