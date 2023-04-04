#include "stm32l476xx.h"

float x = 3.0;
unsigned int a,b,c;

int main () {
	int i;
	// declare pointer to the floating point number x
	char *ptr = (char *)&x;

	// set bits 15 and 14 of variable a to 01 leaving all other bits unchanged
	a = 0xffffffff;
	// a address = 0x20000068
	a &= 0xffff7fff;
	
// reset bits 16 and 15 of b to 0
	b = 0x00018000;
	b &= 0x00000000;
	// b address = 0x2000006C, R2

// replace byte 1 of variable b with the value 0x80 leaving the other bytes unchanged
	b |= 0x00008000;

// shift right variable b by 4 bits, what is the resulting value?
	b = b >> 4;
	
// shift left variable b by 3 bits, what is the resulting value?
	b = b << 3;
	
// obtain the 1s complement of c
	c = 0x155;
	// c address = 0x20000070
	c = ~c;
		
// obtain the 2s complement of c
	c = 0x8555;
	c = ~c;
	c += 1;

// toggle the 16th bit of b ON and OFF
	i=0;
	while (i < 5) {
			// XOR value 00010000
		b ^= 0x00010000;
		i++;
	}

// Determine the address pointed to by ptr[3]
	// 0x20000000, address of x, R1


// Explain the following operation
	ptr[3] |= 0x80;
	// changes x value from 40400000 (3) to C0400000 (-3)
	// this points to the 3rd byte of the ptr that is previously defined and preforms the operation 
	// 0th, 1st, 2nd, 3rd byte
	// preform the operation at the pointer address

	
// Explain the following operation
	*(ptr+2) |= 0x80;
	// changes x value from C0400000 to C0C00000 (-6)
	// points to the 0+2th byte of the previously defined ptr and preforms the operation. 
		
	return 0;
}
