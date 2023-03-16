#include "stm32l476xx.h"

unsigned int a, b, c;
float x = 3.0;

int main () {
// declare pointer to the floating point number x
	char *ptr = (char *)&x;

// set bits 15 and 14 of variable a to 01 leaving all other bits unchanged
	a = 0xffffffff;
	

// reset bits 16 and 15 of b to 0
	b = 0x00018000;


// replace byte 1 of variable b with the value 0x80 leaving the other bytes unchanged


// shift right variable b by 4 bits, what is the resulting value?

	
// shift left variable b by 3 bits, what is the resulting value?


	
// obtain the 1s complement of c
	c = 0x155;
	
		
// obtain the 2s complement of c
	c = 0x8555;


// toggle the 16th bit of b ON and OFF
	while (1) {

	}


// Determine the address pointed to by ptr[3]




// Explain the following operation
	ptr[3] |= 0x80;
	

	
// Explain the following operation
	*(ptr+2) |= 0x80;
	
		
	return 0;
}
