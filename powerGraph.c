#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"

PADIQUE padique(unsigned long n, unsigned char p){
	/*Given a non negative integer n, return n written in base p*/
	unsigned long i;
	unsigned long m = 0, power = 0;
	PADIQUE padiqueN;
	padiqueN.nbBits = (unsigned long)floor(log(n)/log(p)) + 1;

	padiqueN.padique = (unsigned char*)malloc(padiqueN.nbBits * sizeof(unsigned char));
	if(padiqueN.padique == NULL){
		fprintf(stderr, "No memory left.\n");
		exit(EXIT_FAILURE);}
	if(padiqueN.nbBits == 0){
		padiqueN.padique = NULL;
		return padiqueN;}
	else if(padiqueN.nbBits == 1){
		padiqueN.padique[0] = n;
		return padiqueN;}
	else{
		power = pow(p,padiqueN.nbBits-1);
		for(i = padiqueN.nbBits ; i > 0 ; i--){
			padiqueN.padique[m-i] = n / power;
			n -= padiqueN.padique[m-i] * power;
			power = power / p;}
	return padiqueN;}}
