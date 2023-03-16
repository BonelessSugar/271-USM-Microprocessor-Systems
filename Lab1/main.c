#include "stm32l476xx.h"

//static char a[] = "The quick brown fox jumps over the lazy dog!";
static char a[] = "abcd ABCD 0123 ()?!\n\t";
static short b = 255;
static int c = 256;
static float d[] = {-1., -0., 0., 0.5, 1., 1.5, 2.};

int mystrlen(char *s) {
	int n;
	for (n = 0; *s != '\0'; s++) {
		n++;
	}
	return(n);
}

int main(void) {
	static char *pa;
	static short *pb;
	static int *pc;
	static float *pd;
	int n;
	
	pa = a;
	pb = &b;
	pc = &c;
	pd = d;
	n = mystrlen(pa);
	c = n;
	pa = &a[n];
	
	while(1);
		
}
