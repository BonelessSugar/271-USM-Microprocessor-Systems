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
}



int main(void){
	configure_Clock_pin(); // configured as output to display to O-scope. 
	while(1);
}