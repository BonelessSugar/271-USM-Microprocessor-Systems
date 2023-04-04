#include "stm32l476xx.h"

// PA.8 = Clock Output w/ AF-0. 

void configure_Clock_pin(){
// Enable the clock to GPIO Port A
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC -> AHB2ENR |= 0x00000001;
	//output 
  GPIOA -> MODER &= ~(3UL<<16);    
	GPIOA -> MODER |= 2UL<<16;           // 01 general output , 10 alternate function   	
  GPIOA -> OTYPER &= ~(1UL<<8);
  GPIOA -> OSPEEDR &= ~(3UL<<16); 
  GPIOA -> PUPDR &= ~(3<<16);
	GPIOA -> AFR[1] &= ~(7<<0); // fill value with 0000 (for AF0) (pg311/1903)
	RCC -> CFGR &= ~(7<<24);
	RCC -> CFGR |= (2<<24); // MCOSEL[2:0] = 0010 // MSI clock selected (pg 227/1903)
	RCC -> CR &= (0<<0); // MSION is OFF, bit 0 (226/1903)
	RCC -> CR |= (1<<1); // MSI is ready, bit 1 (225/1903)
	RCC -> CR |= (1<<3); // bit 3, set to 1: MSIRGSEL, MSI Range is provided by MSIRANGE[3:0] in the RCC_CR register (225/1903)
	RCC -> CR = 0x00000063; // reset value for CR (223/1903)
	RCC -> CR &= ~(15<<4); // MSIRANGE, bits 4 - 7, range 0 around 100kHz when MSIRGSEL is set (225/1903)
		// and MSION is OFF or MSI is ready
		// lowest frequency at 0
	RCC -> CR |= (11<<4);
		// highest frequency at 11, need to do autoset for it to show, with no single
}



int main(void){
	configure_Clock_pin(); // configured as output to display to O-scope. 
	while(1);
}