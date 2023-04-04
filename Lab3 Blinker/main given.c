#include "stm32l476xx.h"

// PA.5  < --> Green LED

void configure_LED_pin () {
// Enable the clock to GPIO Port A	
  	RCC -> AHB2ENR = 0x????????;
      
// Set the GPIO Port A mode to output  
    GPIOA -> MODER = 0x????????;
             	
// Set the GPIO Port A output type to push - pull 
    GPIOA -> OTYPER = 0x????????;    
 	
// Set the GPIO speed to low
    GPIOA -> OSPEEDR = 0x????????;
 
// Set GPIO Port A Push - Pull to no pull - up, no pull - down
    GPIOA -> PUPDR = 0x????????;
}

int main (void) {
  int i;
  configure_LED_pin();

// turn on LED
  GPIOA -> ODR = ?;
   	
// Dead loop & program hangs here
    	while (1) {
     		for (i = 0; i < 1000000; i++);
// toggle LED
				GPIOA -> ODR = ?;
     	}
  }
