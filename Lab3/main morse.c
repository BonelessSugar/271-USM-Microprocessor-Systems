#include "stm32l476xx.h"

// PA.5  < --> Green LED

void configure_LED_pin () {
// Enable the clock to GPIO Port A
	//textbook pg356
// 0x4002 104C to 0x0000 0001
// |= 1
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  	RCC -> AHB2ENR |= 0x00000001;
      
// Set the GPIO Port A mode to output  
// 0x4800 0000 to 0x5555 5555
	// = 0x55555555
	
    GPIOA -> MODER &= ~(3UL<<10);
	 GPIOA -> MODER |= 1UL<<10; //should we get rid of UL ?
             	
// Set the GPIO Port A output type to push - pull 
//0x4800 0008 to 0x0000 0800
// &= ~(1<<5)
    GPIOA -> OTYPER &= ~(1UL<<5);
	
	//0x00000000;    
 	
// Set the GPIO speed to low
//0x4800 000C to 0x0000 0000
// |= (1<<11)
    GPIOA -> OSPEEDR &= ~(3UL<<10); //could we get rid of UL?
		//0x00000000;
 
// Set GPIO Port A Push - Pull to no pull - up, no pull - down
// &= ~((1<<10) | (1<<11))
    GPIOA -> PUPDR &= ~(3<<10);
		//0x00000000;
}

void morseOneSec () {
	int i;
	for (i = 0; i < 125000; i++);
	GPIOA -> ODR ^= 1UL<<5;
}

void morseDot () {
	int i;
	GPIOA -> ODR |= (1UL<<5);
	for (i = 0; i < 125000 / 2; i++);
	GPIOA -> ODR &= ~(1UL<<5);
	for (i = 0; i < 125000 / 2; i++);
}

void morseDash () {
	int i;
	GPIOA -> ODR |= (1UL<<5);
	for (i = 0; i < 125000 * 3 / 2; i++);
	GPIOA -> ODR &= ~(1UL<<5);
	for (i = 0; i < 125000 / 2; i++);
}

void betweenLetter () {
	int i;
	GPIOA -> ODR &= ~(1UL<<5);
	for (i = 0; i < 125000 * 2 / 2; i++);
}

void betweenWord () {
	int i;
	GPIOA -> ODR &= ~(1UL<<5);
	for (i = 0; i < 125000 * 6 / 2; i++);
}

int main (void) {
  configure_LED_pin();

// turn on LED
	// GPIOA -> ODR |= 1<<5;
  GPIOA -> ODR |= 1<<5;
   	
// Dead loop & program hangs here
	// you have to click the black button on the board for it to start running
    	while (1) {
				morseDot();
				morseDot();
				morseDot();
				betweenLetter();
				morseDash();
				morseDash();
				morseDash();
				betweenLetter();
				morseDot();
				morseDot();
				morseDot();
				betweenWord();
			}
		}
